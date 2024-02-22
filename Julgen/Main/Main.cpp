#include <SDL.h>

#if _DEBUG
	// ReSharper disable once CppUnusedIncludeDirective
	#if __has_include(<vld.h>)
	#include <vld.h>
	#endif
#endif

#include <cstdio>

#include "GlobalSettings.h"
#include "Julgen.h"
#include "SDL_egl.h"

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



