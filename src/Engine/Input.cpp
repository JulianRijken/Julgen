#include "Input.h"

#include <imgui_impl_sdl2.h>
#include <SDL.h>

#include "GUI.h"


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
        if(EngineGUI::ProcessEvent(&event))
            continue;
    }
}

void jul::Input::HandleKeyboardContinually() const
{
    const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);

    for (auto&& bind : m_Binds)
    {
        if(not bind.allowKeyboard)
            continue;

        for (auto&& keyboardKey : bind.acton.keyboardButtons)
        {
            if(bind.state == ButtonState::Held and keyboardState[keyboardKey])
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
            if(not bind.allowKeyboard)
               continue;

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
    if(state != checkButtonState)
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
    if(state != checkButtonState)
        return false;

    if(not acton.HasKeyboardKey(compareKey))
        return false;

    command->Execute();
    return true;
}

void jul::Input::HandleControllerContinually()
{
    UpdateControllers();

    for(auto&& bind : m_Binds)
    {
        if(bind.state != ButtonState::Held)
            continue;

        // Get the controller and check if it exists
        if(SDL_GameController* controller = GetController(bind.controllerIndex))
        {
            // Handle all controller buttons
            for(auto&& controllerButton : bind.acton.controllerButtons)
            {
                if(SDL_GameControllerGetButton(controller, controllerButton))
                {
                    bind.command->Execute();
                    break;
                }
            }

            // Handle right stick
            if(bind.acton.HasControllerAxis(SDL_CONTROLLER_AXIS_RIGHTX) and
               bind.acton.HasControllerAxis(SDL_CONTROLLER_AXIS_RIGHTY))
            {
                bind.command->Execute(glm::vec2{
                    Input::NormalizeAxis(
                        SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX), STICK_DEADZONE, AXIS_LIMIT),
                    -Input::NormalizeAxis(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY),
                                          STICK_DEADZONE,
                                          AXIS_LIMIT) });
                continue;
            }

            // Handle left stick
            if(bind.acton.HasControllerAxis(SDL_CONTROLLER_AXIS_LEFTX) and
               bind.acton.HasControllerAxis(SDL_CONTROLLER_AXIS_LEFTY))
            {
                bind.command->Execute(glm::vec2{
                    Input::NormalizeAxis(
                        SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX), STICK_DEADZONE, AXIS_LIMIT),
                    -Input::NormalizeAxis(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY),
                                          STICK_DEADZONE,
                                          AXIS_LIMIT) });
                continue;
            }

            // Handle all remaining axis
            for(auto&& controllerAxis : bind.acton.controllerAxis)
                bind.command->Execute(Input::NormalizeAxis(
                    SDL_GameControllerGetAxis(controller, controllerAxis), STICK_DEADZONE, AXIS_LIMIT));
        }
    }
}

bool jul::Input::HandleControllerEvent(const SDL_Event& event)
{
    switch(event.type)
    {
        case SDL_CONTROLLERBUTTONDOWN:
        case SDL_CONTROLLERBUTTONUP:
            const ButtonState buttonState =
                event.type == SDL_CONTROLLERBUTTONDOWN ? ButtonState::Down : ButtonState::Up;

            for(auto&& bind : m_Binds)
                bind.TryExecuteController(
                    buttonState, event.cbutton.which, static_cast<SDL_GameControllerButton>(event.cbutton.button));

            return true;
    }

    return false;
}

void jul::Input::UpdateControllers()
{

    const int joystickCount = SDL_NumJoysticks();

    if(static_cast<int>(m_Controllers.size()) < joystickCount)
        m_Controllers.resize(joystickCount, nullptr);

    for(int i = 0; i < joystickCount; i++)
    {
        if(not SDL_IsGameController(i))
            m_Controllers[i] = nullptr;

        if(m_Controllers[i] != nullptr)
            continue;

        m_Controllers[i] = SDL_GameControllerOpen(i);
    }
}

SDL_GameController* jul::Input::GetController(int controllerIndex)
{

    // Check if controller is out of bounds
    if(controllerIndex >= static_cast<int>(m_Controllers.size()))
        return nullptr;

    return m_Controllers[controllerIndex];
}
