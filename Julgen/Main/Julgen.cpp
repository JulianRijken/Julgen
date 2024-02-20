#include <stdexcept>
#include <sstream>
#include <iostream>

#if WIN32
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#endif

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Julgen.h"

#include <thread>

#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "GlobalSettings.h"
#include "InputManager.h"
#include "Timing.h"


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

#ifdef __EMSCRIPTEN__
#include "emscripten.h"

void LoopCallback(void* arg)
{
	static_cast<jul::Julgen*>(arg)->RunOneFrame();
}
#endif

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

	Renderer::GetInstance().Init(g_window);


#if __EMSCRIPTEN__
	fs::path contentLocation = "";
#else
	std::filesystem::path contentLocation = "./";

	if (not std::filesystem::exists(contentLocation))
		contentLocation = "../";

	contentLocation.append(GlobalSettings::CONTENT_PATH);
#endif
	
	ResourceManager::GetInstance().Init(contentLocation);
}

jul::Julgen::~Julgen()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

void jul::Julgen::Run(const std::function<void()>& load)
{

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop_arg(&LoopCallback, this, 0, true);
#else

#ifdef WIN32
	if constexpr (GlobalSettings::SHOW_CONSOLE)
	{
		if (AllocConsole())
		{
			FILE* empty;
			freopen_s(&empty, "CONOUT$", "w", stdout);
			freopen_s(&empty, "CONOUT$", "w", stderr);
		}
	}
#endif


	load();


	auto lastTime = std::chrono::high_resolution_clock::now();

	double lag = 0.0;

	while (not m_IsApplicationQuitting)
	{
		// Handle delta time
		auto currentTime = std::chrono::high_resolution_clock::now();
		const double deltaTime = std::chrono::duration<double>(currentTime - lastTime).count();
		lastTime = currentTime;
		lag += deltaTime;
		Timing::SetDeltaTime(deltaTime);


		// Fixed Update
		m_IsApplicationQuitting = !InputManager::GetInstance().ProcessInput();
		while (lag >= Timing::FIXED_TIME_STEP)
		{
			SceneManager::GetInstance().FixedUpdate();

			lag -= Timing::FIXED_TIME_STEP;
		}

		// Update
		SceneManager::GetInstance().Update();
		// Late Update
		SceneManager::GetInstance().LateUpdate();

		// Render
		Renderer::GetInstance().Render();


		Timing::AddToFrameCount();

		std::cout << 1.0 / Timing::GetDeltaTime() << '\n';

		// Vsync should be enabled!
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

#endif


}

