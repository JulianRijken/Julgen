#include "Julgen.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <stdexcept>

#include "Achievement.h"
#include "EngineGUI.h"
#include "GameSettings.h"
#include "GameTime.h"
#include "Input.h"
#include "Locator.h"
#include "MessageQueue.h"
#include "Physics.h"
#include "RenderManager.h"
#include "Resources.h"
#include "SceneManager.h"
#include "Sound.h"
#include "TweenEngine.h"
#include "Sound_System.h"

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
    if(GameSettings::g_ShowConsole)
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


    m_Window = SDL_CreateWindow(GameSettings::g_WindowTitle.c_str(),
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                GameSettings::g_WindowWidth,
                                GameSettings::g_WindowHeight,
                                SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if(m_Window == nullptr)
        throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());

    Locator::Provide<Physics>();
    Locator::Provide<Sound, Sound_System>();

    RenderManager::GetInstance().Initialize(m_Window);
    Achievement::GetInstance().Initialize();
    EngineGUI::Initialize(m_Window, RenderManager::GetInstance().GetSDLRenderer());
    Resources::Initialize();

    GameStart();

    // Allow default scene to load before the game loop
    // required as scenes only load at the end of the frame
    SceneManager::GetInstance().LoadScenesSetToLoad();

    Run();
}

jul::Julgen::~Julgen()
{
    SceneManager::GetInstance().Destroy();

    Resources::Destroy();
    EngineGUI::Destory();
    RenderManager::GetInstance().Destroy();

    Locator::Release<Sound>();
    Locator::Release<Physics>();

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

// Execution order:
// > Input Events
// > Fixed Update
// > Update
// > Message Dispatch
// > Tween Update
// > Late Update
// > Render
// > Cleanup / Load

void jul::Julgen::RunOneFrame()
{
#ifdef USE_STEAMWORKS
    SteamAPI_RunCallbacks(); // Valve Please Run :)
#endif

    // GameTimeUpdate
    g_LoopState = LoopState::GameTimeUpdate;
    GameTime::Update();
    m_Lag += GameTime::GetDeltaTime();

    // ProcessInput
    g_LoopState = LoopState::ProcessInput;
    Input::GetInstance().ProcessInput(m_IsApplicationQuitting);

    // FixedUpdate
    while (m_Lag >= GameTime::GetFixedDeltaTime())
	{
        g_LoopState = LoopState::FixedUpdateActive;
        Locator::Get<Physics>().UpdateIsActive();

        g_LoopState = LoopState::FixedUpdate;
        SceneManager::GetInstance().FixedUpdate();

        g_LoopState = LoopState::FixedUpdateWorld;
        Locator::Get<Physics>().UpdateWorld();

        m_Lag -= GameTime::GetFixedDeltaTime();
    }

    // Update
    g_LoopState = LoopState::Update;
    SceneManager::GetInstance().Update();

    // Message Dispatch
    g_LoopState = LoopState::MessageDispatch;
    MessageQueue::Dispatch();

    // Tween Update
    g_LoopState = LoopState::Update;
    TweenEngine::GetInstance().Update();

    // Late Update
    g_LoopState = LoopState::LateUpdate;
    SceneManager::GetInstance().LateUpdate();

    // Render
    g_LoopState = LoopState::Render;
    RenderManager::GetInstance().PickCamera();
    RenderManager::GetInstance().Render();

    // Load / Unload Game Objects and Scenes
    // Loop state happends inside of process scenes
    SceneManager::GetInstance().ProcessScenes();

    g_LoopState = LoopState::AddingFrameCount;
    GameTime::AddToFrameCount();
}
