#include "Input.h"

#include <SDL.h>
#include <imgui_impl_sdl2.h>



bool jul::Input::ProcessInput()
{
    HandleKeyboardContinually();
    HandleControllerContinually();

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
            return false;

        if(HandleKeyboardEvent(event))
            continue;

        if(HandleControllerEvent(event))
            continue;

        // TODO: Should be before the other input
        // But it should only take input when focused
        // No clue why it always takes keyboard events now ¯\_(ツ)_/¯
        if(ImGui_ImplSDL2_ProcessEvent(&event))
            continue;
    }

    return true;
}

void jul::Input::HandleKeyboardContinually()
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
}

bool jul::Input::HandleKeyboardEvent(const SDL_Event& event)
{
    switch (event.type)
    {
    case SDL_KEYDOWN:

        for (auto&& bind : m_Binds)
            bind.TryExcecuteKeyboard(ButtonState::Down,event.key.keysym.scancode);

        return true;

    case SDL_KEYUP:

        for (auto&& bind : m_Binds)
             bind.TryExcecuteKeyboard(ButtonState::Up,event.key.keysym.scancode);

        return true;
    }
    return false;
}

