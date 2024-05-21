#include "Input.h"

#include "EngineGUI.h"

// TODO: Maybe have should quit be a return value?
void jul::Input::ProcessInput(bool& shouldQuit)
{
    UpdateControllers();

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
        {
            shouldQuit = true;
            continue;
        }


        if(HandleControllerEvent(event))
            continue;

        if(HandleKeyboardEvent(event))
            continue;

        // TODO: Should be before the other input
        // But it should only take input when focused
        // No clue why it always takes keyboard events now ¯\_(ツ)_/¯
        if(EngineGUI::ProcessEvent(&event))
            continue;
    }
}


bool jul::Input::HandleKeyboardEvent(const SDL_Event& event) const
{
    switch(event.type)
    {
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            const ButtonState buttonState = event.type == SDL_KEYDOWN ? ButtonState::Down : ButtonState::Up;
            for(auto&& bind : m_Binds)
            {
                if(not bind.allowKeyboard)
                    continue;

                if(event.key.repeat)
                    continue;

                if(bind.TryExecuteKeyboard(buttonState, event.key.keysym.scancode))
                    continue;
            }
            return true;
    }
    return false;
}

bool jul::Input::HandleControllerEvent(const SDL_Event& event) const
{
    const int controllerIndex = static_cast<int>(event.jdevice.which);
    if(not SDL_IsGameController(controllerIndex))
        return false;

    switch(event.type)
    {
        case SDL_CONTROLLERBUTTONDOWN:
        case SDL_CONTROLLERBUTTONUP:
        {
            const ButtonState buttonState =
                event.type == SDL_CONTROLLERBUTTONDOWN ? ButtonState::Down : ButtonState::Up;

            for(auto&& bind : m_Binds)
                bind.TryExecuteController(
                    buttonState, controllerIndex, static_cast<SDL_GameControllerButton>(event.cbutton.button));

            return true;
        }
        case SDL_CONTROLLERAXISMOTION:
        {
            HandleControllerAxis(static_cast<SDL_GameControllerAxis>(event.caxis.axis));
        }
        break;
    }

    return false;
}

void jul::Input::HandleControllerAxis(const SDL_GameControllerAxis& axis) const
{
    for(auto&& bind : m_Binds)
    {
        // Get the controller and check if it exists
        if(SDL_GameController* controller = GetController(bind.controllerIndex))
        {
            if(axis == SDL_CONTROLLER_AXIS_RIGHTX or axis == SDL_CONTROLLER_AXIS_RIGHTY)
            {
                // Handle right stick
                if(bind.acton.HasControllerAxis(SDL_CONTROLLER_AXIS_RIGHTX) and
                   bind.acton.HasControllerAxis(SDL_CONTROLLER_AXIS_RIGHTY))
                {
                    bind.command->Execute({
                        ButtonState::Down,
                        glm::vec2{
                                  Input::NormalizeAxis(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX),
                                  STICK_DEADZONE, AXIS_LIMIT),
                                  -Input::NormalizeAxis(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY),
                                  STICK_DEADZONE, AXIS_LIMIT)}
                    });
                    continue;
                }
            }

            if(axis == SDL_CONTROLLER_AXIS_LEFTX or axis == SDL_CONTROLLER_AXIS_RIGHTX)
            {
                // Handle left stick
                if(bind.acton.HasControllerAxis(SDL_CONTROLLER_AXIS_LEFTX) and
                   bind.acton.HasControllerAxis(SDL_CONTROLLER_AXIS_LEFTY))
                {
                    bind.command->Execute({
                        ButtonState::Down,
                        glm::vec2{
                                  Input::NormalizeAxis(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX),
                                  STICK_DEADZONE, AXIS_LIMIT),
                                  -Input::NormalizeAxis(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY),
                                  STICK_DEADZONE, AXIS_LIMIT)}
                    });
                    continue;
                }
            }

            // Handle all remaining axis
            for(auto&& controllerAxis : bind.acton.controllerAxis)
                bind.command->Execute(
                    { ButtonState::Down,
                      Input::NormalizeAxis(
                          SDL_GameControllerGetAxis(controller, controllerAxis), STICK_DEADZONE, AXIS_LIMIT) });
        }
    }
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

SDL_GameController* jul::Input::GetController(int controllerIndex) const
{

    // Check if controller is out of bounds
    if(controllerIndex >= static_cast<int>(m_Controllers.size()))
        return nullptr;

    return m_Controllers[controllerIndex];
}
