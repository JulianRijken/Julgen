#include <SDL.h>

#if _DEBUG
	// ReSharper disable once CppUnusedIncludeDirective
	#if __has_include(<vld.h>)
	#include <vld.h>
	#endif
#endif

#include "Julgen.h"

using namespace jul;

int main(int, char*[])
{
	Julgen engine{};
	engine.Run();
    return 0;
}



