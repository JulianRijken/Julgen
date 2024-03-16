#include "Input.h"

#include <SDL.h>
#include <imgui_impl_sdl2.h>



bool jul::Input::ProcessInput()
{
    auto keyboardState = SDL_GetKeyboardState(nullptr);
    for (auto&& bind : m_Binds)
    {
        for (auto&& keyboardKey : bind.acton.keyboardButtons)
        {
            if(bind.buttonState == ButtonState::Held and keyboardState[keyboardKey])
            {
                bind.command->Execute();
                break;
            }
        }
    }


    HandleController();


    SDL_Event event;
    while (SDL_PollEvent(&event))
    {

        switch (event.type)
        {

        case SDL_QUIT:
            return false;

        case SDL_KEYDOWN:
            for (auto&& bind : m_Binds)
            {
                if(bind.buttonState != ButtonState::Down)
                    continue;

                if(bind.acton.HasKeyboardKey(event.key.keysym.scancode))
                    bind.command->Execute();
            }
            break;
        case SDL_KEYUP:
            for (auto&& bind : m_Binds)
            {
                if(bind.buttonState != ButtonState::Up)
                    continue;

                if(bind.acton.HasKeyboardKey(event.key.keysym.scancode))
                    bind.command->Execute();
            }
            break;


        case SDL_CONTROLLERBUTTONDOWN:
            for (auto&& bind : m_Binds)
            {
                if(bind.buttonState != ButtonState::Down)
                    continue;

                if(bind.acton.HasControllerButton(static_cast<SDL_GameControllerButton>(event.cbutton.button)))
                    bind.command->Execute();
            }
            break;

        case SDL_CONTROLLERBUTTONUP:
            for (auto&& bind : m_Binds)
            {
                if(bind.buttonState != ButtonState::Up)
                    continue;

                if(bind.acton.HasControllerButton(static_cast<SDL_GameControllerButton>(event.cbutton.button)))
                    bind.command->Execute();
            }
            break;
        }


        ImGui_ImplSDL2_ProcessEvent(&event);
    }

    return true;
}
