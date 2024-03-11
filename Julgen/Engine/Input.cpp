#include "Input.h"

#include <SDL.h>
#include <imgui_impl_sdl2.h>

#include "Command.h"


bool jul::Input::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) 
	{
		if (event.type == SDL_QUIT) 
			return false;

		
		if (event.type == SDL_KEYDOWN) 
		{
			
		}
		if (event.type == SDL_MOUSEBUTTONDOWN) 
		{
			
		}
		if (event.type == SDL_MOUSEBUTTONDOWN)
		{

		}




		ImGui_ImplSDL2_ProcessEvent(&event);
	}

	return true;
}
