#include "Julgen.h"

#include <stdexcept>
#include <sstream>
#include <thread>
#include <iostream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "GameTime.h"
#include "GlobalSettings.h"
#include "GUI.h"
#include "Input.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "MessageQueue.h"
#include "Achievement.h"

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


SDL_Window* g_window{};

jul::Julgen::Julgen()
{
    #ifdef USE_STEAMWORKS
        if(not SteamAPI_Init())
            std::runtime_error("Fatal Error - Steam must be running to play this game (SteamAPI_Init failed).");
        else
            std::cout << "Steam successfully initialized!" << std::endl;
    #endif



    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0)
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());


	g_window = SDL_CreateWindow(
		GlobalSettings::WINDOW_TITLE,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		GlobalSettings::WINDOW_WIDTH,
		GlobalSettings::WINDOW_HEIGHT,
		SDL_WINDOW_OPENGL
	);

	if (g_window == nullptr)
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());

	RenderManager::GetInstance().Initialize(g_window);
    EngineGUI::GetInstance().Initialize(g_window,RenderManager::GetInstance().GetSDLRenderer());
    Achievement::GetInstance().Initialize();

    ResourceManager::Initialize();
}

jul::Julgen::~Julgen()
{
    EngineGUI::GetInstance().Destroy();
	RenderManager::GetInstance().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;

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
	// TODO: Replace this hardcoded start scene
	SceneManager::GetInstance().LoadScene("Start"); 

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

