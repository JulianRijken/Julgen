#include <SDL.h>
#include <cstdio>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

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



