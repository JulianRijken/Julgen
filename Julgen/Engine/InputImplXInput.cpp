#include "Input.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <XInput.h>
#include <iostream>
#include <Thread>


namespace jul
{
    class Input::InputImpl
    {

    public:

        SDL_GameController* FindController()
        {
            for (int i = 0; i < SDL_NumJoysticks(); i++)
            {
                if (SDL_IsGameController(i))
                    return SDL_GameControllerOpen(i);
            }

            return nullptr;
        }

        void HandleController()
        {
            currentState = {};
            XInputGetState(controllerIndex, &currentState);

            for (auto&& bind : Input::GetInstance().m_Binds)
            {
                if(bind.buttonState == ButtonState::Held )
                {
                    for (auto&& controllerButton : bind.acton.controllerButtons)
                    {
                        if(currentState.Gamepad.wButtons == SDLButtonToXInput(controllerButton))

                        bind.command->Execute();
                        break;
                    }
                }
            }
        }

    private:

        XINPUT_STATE currentState{};
        int controllerIndex{};

        // As our engine always uses SDL we use a SDL to XInput mapper to keep the interface for the user the same and our code simple :)
        // Alos XInput might get removed from the engine in the futuer so this keeps it seperate
        WORD SDLButtonToXInput(SDL_GameControllerButton sdlButton) {
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

    };
}

void jul::Input::HandleController()
{
    m_ImplUPtr->HandleController();
}

jul::Input::Input() :
      m_ImplUPtr(std::make_unique<InputImpl>())
{
};

jul::Input::~Input() = default;

