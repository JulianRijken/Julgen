#include <SDL.h>
#include <SDL_egl.h>
#include <cstdio>

#if _DEBUG
	// ReSharper disable once CppUnusedIncludeDirective
	#if __has_include(<vld.h>)
	#include <vld.h>
	#endif
#endif

#include "GlobalSettings.h"
#include "Julgen.h"

using namespace jul;

int main(int, char*[])
{
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

	Julgen engine{};
	engine.Run();
    return 0;
}



