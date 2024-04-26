#define SDL_MAIN_HANDLED
#include <SDL.h>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#if _DEBUG
	#if __has_include(<vld.h>)
	#include <vld.h>
	#endif
#endif

#include "Julgen.h"

int main(int, char*[])
{
    const jul::Julgen engine;
    return 0;
}
