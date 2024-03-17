#include "Input.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <XInput.h>
#include <iostream>
#include <Thread>


namespace jul
{
class Input::ControllerInputImpl
    {
    public:

        void HandleControllerContinually(const std::vector<InputBinding>& binds)
        {
            UpdateControllerInfo();

            for (auto&& bind : binds)
            {
                for (auto&& controllerButton : bind.acton.controllerButtons)
                {
                    if(bind.buttonState == ButtonState::Held and IsHeld(SDLButtonToXInput(controllerButton),bind.controllerIndex))
                    {
                        bind.command->Execute();
                        break; // We break for the hold to avoid excecuting twice
                    }
                    else
                    if(bind.buttonState == ButtonState::Down and IsDownThisFrame(SDLButtonToXInput(controllerButton),bind.controllerIndex))
                        bind.command->Execute(); // We do not need this break for the down as we do want it to execute twice
                    else
                    if(bind.buttonState == ButtonState::Up and IsUpThisFrame(SDLButtonToXInput(controllerButton),bind.controllerIndex))
                        bind.command->Execute();
                }
            }
        }

    private:

        struct XINPUT_GAMEPAD_STATE
        {
            XINPUT_STATE currentControllerState;
            XINPUT_STATE priviousControllerState;
            WORD buttonsPressedThisFrame;
            WORD buttonsReleasedThisFrame;
        };

        bool IsDownThisFrame(WORD button, int controllerIndex = 0) const
        {
            return m_ControllerStates[controllerIndex].buttonsPressedThisFrame & button;
        }
        bool IsUpThisFrame(WORD button, int controllerIndex = 0) const
        {
            return m_ControllerStates[controllerIndex].buttonsReleasedThisFrame & button;
        }
        bool IsHeld(WORD button, int controllerIndex = 0) const
        {
            return m_ControllerStates[controllerIndex].currentControllerState.Gamepad.wButtons & button;
        }

        void UpdateControllerInfo()
        {
            int joystickCount = SDL_NumJoysticks();
            m_ControllerStates.resize(joystickCount);

            for (int controllerIndex = 0; controllerIndex < joystickCount; controllerIndex++)
            {
                XINPUT_GAMEPAD_STATE& gamepadState = m_ControllerStates[controllerIndex];

                // Set privious state
                gamepadState.priviousControllerState = gamepadState.currentControllerState;

                // Set new flags;
                gamepadState.currentControllerState = {};
                XInputGetState(controllerIndex, &gamepadState.currentControllerState);

                // Update current frame checks
                WORD buttonChanges = gamepadState.currentControllerState.Gamepad.wButtons ^ gamepadState.priviousControllerState.Gamepad.wButtons;
                gamepadState.buttonsPressedThisFrame = buttonChanges & gamepadState.currentControllerState.Gamepad.wButtons;
                gamepadState.buttonsReleasedThisFrame = buttonChanges & (~gamepadState.currentControllerState.Gamepad.wButtons);
            }
        }


        // As our engine always uses SDL we use a SDL to XInput mapper to keep the interface for the user the same and our code simple :)
        // Alos XInput might get removed from the engine in the futuer so this keeps it fully seperated
        WORD SDLButtonToXInput(SDL_GameControllerButton sdlButton)
        {
            switch (sdlButton) {
            case SDL_CONTROLLER_BUTTON_A:            return XINPUT_GAMEPAD_A;
            case SDL_CONTROLLER_BUTTON_B:            return XINPUT_GAMEPAD_B;
            case SDL_CONTROLLER_BUTTON_X:            return XINPUT_GAMEPAD_X;
            case SDL_CONTROLLER_BUTTON_Y:            return XINPUT_GAMEPAD_Y;
            case SDL_CONTROLLER_BUTTON_BACK:         return XINPUT_GAMEPAD_BACK;
            case SDL_CONTROLLER_BUTTON_START:        return XINPUT_GAMEPAD_START;
            case SDL_CONTROLLER_BUTTON_LEFTSTICK:    return XINPUT_GAMEPAD_LEFT_THUMB;
            case SDL_CONTROLLER_BUTTON_RIGHTSTICK:   return XINPUT_GAMEPAD_RIGHT_THUMB;
            case SDL_CONTROLLER_BUTTON_LEFTSHOULDER: return XINPUT_GAMEPAD_LEFT_SHOULDER;
            case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:return XINPUT_GAMEPAD_RIGHT_SHOULDER;
            case SDL_CONTROLLER_BUTTON_DPAD_UP:      return XINPUT_GAMEPAD_DPAD_UP;
            case SDL_CONTROLLER_BUTTON_DPAD_DOWN:    return XINPUT_GAMEPAD_DPAD_DOWN;
            case SDL_CONTROLLER_BUTTON_DPAD_LEFT:    return XINPUT_GAMEPAD_DPAD_LEFT;
            case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:   return XINPUT_GAMEPAD_DPAD_RIGHT;
            default:
                std::cerr << "No equivalent XInput mapping for the given SDL controller button." << std::endl;
                return 0;
            }
        }

        std::vector<XINPUT_GAMEPAD_STATE> m_ControllerStates{};
    };
}

void jul::Input::HandleControllerContinually(const std::vector<InputBinding>& binds)
{
    m_ImplUPtr->HandleControllerContinually(binds);
}

// XInput does not use events but requires to check the key state every frame
// in order to detect changes in input (simulating events)
bool jul::Input::HandleControllerEvent(const SDL_Event&,const std::vector<InputBinding>&)
{
    return false;
}


jul::Input::Input() :
      m_ImplUPtr(std::make_unique<ControllerInputImpl>())
{};

jul::Input::~Input() = default;

