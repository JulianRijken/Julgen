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
#include "InputManager.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"

#if WIN32
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#endif


SDL_Window* g_window{};

void LogSDLVersion(const std::string& message, const SDL_version& v)
{
#if WIN32
	std::stringstream ss;
	ss << message << (int)v.major << "." << (int)v.minor << "." << (int)v.patch << "\n";
	OutputDebugString(ss.str().c_str());
#else
	std::cout << message << (int)v.major << "." << (int)v.minor << "." << (int)v.patch << "\n";
#endif
}

// Why bother with this? Because sometimes students have a different SDL version installed on their pc.
// That is not a problem unless for some reason the dll's from this project are not copied next to the exe.
// These entries in the debug output help to identify that issue.
void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	LogSDLVersion("We compiled against SDL version ", version);

	SDL_GetVersion(&version);
	LogSDLVersion("We linked against SDL version ", version);

	SDL_IMAGE_VERSION(&version);
	LogSDLVersion("We compiled against SDL_image version ", version);

	version = *IMG_Linked_Version();
	LogSDLVersion("We linked against SDL_image version ", version);

	SDL_TTF_VERSION(&version)
		LogSDLVersion("We compiled against SDL_ttf version ", version);

	version = *TTF_Linked_Version();
	LogSDLVersion("We linked against SDL_ttf version ", version);
}


jul::Julgen::Julgen()
{
	PrintSDLVersion();

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
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

	ResourceManager::Initialize();
}

jul::Julgen::~Julgen()
{
	RenderManager::GetInstance().Destroy();

	SDL_DestroyWindow(g_window);
	g_window = nullptr;

	SDL_Quit();
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
	// Update time
	GameTime::Update();
	m_Lag += GameTime::GetDeltaTime();

	// Handle input
	m_IsApplicationQuitting = !InputManager::GetInstance().ProcessInput();

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
	SceneManager::GetInstance().Cleanup();

	GameTime::AddToFrameCount();

	// Avoid over using resources when VSync is not on
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

