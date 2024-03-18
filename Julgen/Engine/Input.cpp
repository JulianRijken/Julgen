#include "Input.h"

#include <SDL.h>
#include <imgui_impl_sdl2.h>


// TODO: Maybe not handle shouldQuit via a reference
void jul::Input::ProcessInput(bool& shouldQuit)
{
    HandleKeyboardContinually();
    HandleControllerContinually();

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
        {
            shouldQuit = true;
            continue;
        }

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

}

void jul::Input::HandleKeyboardContinually() const
{
    const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);

    for (auto&& bind : m_Binds)
    {
        for (auto&& keyboardKey : bind.acton.keyboardButtons)
        {
            if (bind.buttonState == ButtonState::Held and keyboardState[keyboardKey])
            {    
                bind.command->Execute();
                break;
            }
        }
    }
}

bool jul::Input::HandleKeyboardEvent(const SDL_Event& event) const
{
    switch (event.type)
    {
    case SDL_KEYDOWN:
    case SDL_KEYUP:
       const ButtonState buttonState = event.type == SDL_KEYDOWN ? ButtonState::Down : ButtonState::Up;
        for (auto&& bind : m_Binds)
        {
            if(event.key.repeat)
                continue;

            bind.TryExecuteKeyboard(buttonState,event.key.keysym.scancode);
        }
        return true;
    }
    return false;
}


bool jul::InputBinding::TryExecuteController(
    ButtonState checkButtonState,
    int checkControllerIndex,
    SDL_GameControllerButton compareButton) const
{
    if(buttonState != checkButtonState)
        return false;

    if(controllerIndex != checkControllerIndex)
        return false;

    if(not acton.HasControllerButton(compareButton))
        return false;

    command->Execute();
    return true;
}

bool jul::InputBinding::TryExecuteKeyboard(ButtonState checkButtonState, SDL_Scancode compareKey) const
{
    if(buttonState != checkButtonState)
        return false;

    if(not acton.HasKeyboardKey(compareKey))
        return false;

    command->Execute();
    return true;
}
