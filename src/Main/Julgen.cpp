#include "Julgen.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <stdexcept>
#include <thread>

#include "Achievement.h"
#include "GameSettings.h"
#include "GameTime.h"
#include "GUI.h"
#include "Input.h"
#include "Locator.h"
#include "MessageQueue.h"
#include "Physics.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Sound.h"
#include "Sound_Logging.h"
#include "Sound_SoLoud.h"

#if WIN32
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#endif

#ifdef USE_STEAMWORKS
    #ifdef _MSC_VER
    #pragma warning (push)
    #pragma warning (disable: 4996)
    #endif

    #include <steam_api.h>

    #ifdef _MSC_VER
    #pragma warning (pop)
    #endif

    #ifdef __EMSCRIPTEN__
    #error  Steamworks cant be used in web builds... "Valve please fix" ~3kliksPhilip
    #endif
#endif



jul::Julgen::Julgen()
{
    PreInit();

#ifdef USE_STEAMWORKS
    if(not SteamAPI_Init())
        std::runtime_error("Fatal Error - Steam must be running to play this game (SteamAPI_Init failed).");
    else
        std::cout << "Steam successfully initialized!" << std::endl;
#endif

#ifdef WIN32
    if(GameSettings::s_ShowConsole)
    {
        if(AllocConsole() == TRUE)
        {
            FILE* empty{};
            freopen_s(&empty, "CONOUT$", "w", stdout);
            freopen_s(&empty, "CONOUT$", "w", stderr);
        }
    }
#endif

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0)
        throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());


    m_Window = SDL_CreateWindow(GameSettings::s_WindowTitle.c_str(),
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                GameSettings::s_WindowWidth,
                                GameSettings::s_WindowHeight,
                                SDL_WINDOW_OPENGL);

    if(m_Window == nullptr)
        throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());


    Locator::Provide<Physics>();
    Locator::Provide<Sound, Sound_Logging<Sound_SoLoud>>();

    // TODO: REMOVE
    // THIS IS AN EXAMPLE ON HOW TO SWTICH BETWEEN LOGGING
    {
        // We start as logging above

        // Change to non logigng
        auto soundLoggingUPtr = Locator::Release<Sound, Sound_Logging<Sound_SoLoud>>();
        Locator::Provide<Sound, Sound_SoLoud>(soundLoggingUPtr->ReleaseSoundSystem());

        // Change to logging
        Locator::Provide<Sound, Sound_Logging<Sound_SoLoud>>(Locator::Release<Sound, Sound_SoLoud>());
    }


    RenderManager::GetInstance().Initialize(m_Window);
    Achievement::GetInstance().Initialize();


    EngineGUI::Initialize(m_Window, RenderManager::GetInstance().GetSDLRenderer());
    ResourceManager::Initialize();

    GameStart();
    Run();
}

jul::Julgen::~Julgen()
{
    ResourceManager::Destroy();
    EngineGUI::Destroy();
    RenderManager::GetInstance().Destroy();
    SDL_DestroyWindow(m_Window);
    m_Window = nullptr;

    SDL_Quit();

#ifdef USE_STEAMWORKS
    SteamAPI_Shutdown();
#endif
}


#ifdef __EMSCRIPTEN__
#include "emscripten.h"

void LoopCallback(void* arg)
{
	static_cast<jul::Julgen*>(arg)->RunOneFrame();
}
#endif

void jul::Julgen::Run()
{
#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop_arg(&LoopCallback, this, 0, true);
#else

	while (not m_IsApplicationQuitting)
		RunOneFrame();
#endif
}

void jul::Julgen::RunOneFrame()
{
#ifdef USE_STEAMWORKS
    SteamAPI_RunCallbacks(); // Valve Please Run :)
#endif

	// Update time
	GameTime::Update();
	m_Lag += GameTime::GetDeltaTime();

    MessageQueue::Dispatch();

    // Handle input
    Input::GetInstance().ProcessInput(m_IsApplicationQuitting);

	// Fixed Update,
	while (m_Lag >= GameTime::GetFixedDeltaTime())
	{
        SceneManager::GetInstance().FixedUpdate();
        Locator::Get<Physics>().FixedUpdate();  // TODO: Needs cache?
        m_Lag -= GameTime::GetFixedDeltaTime();
	}

	// Update
	SceneManager::GetInstance().Update();
	// Late Update
	SceneManager::GetInstance().LateUpdate();


	// Render
	RenderManager::GetInstance().Render();

	// Cleans up all the objects marked for deletion
	SceneManager::GetInstance().CleanupGameObjects();

	GameTime::AddToFrameCount();

	// Avoid over using resources when VSync is not on
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

