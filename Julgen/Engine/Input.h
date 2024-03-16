#pragma once
#include <SDL.h>
#include <SDL_keycode.h>
#include <algorithm>
#include <map>
#include <memory>
#include <vector>

#include "Singleton.h"
#include "Command.h"

namespace jul
{

    struct InputAction
    {
        bool HasKeyboardKey(SDL_Scancode compareKey)
        {
            return std::ranges::count(keyboardButtons,compareKey) > 0;
        }

        bool HasControllerButton(Uint8 compareButton)
        {
            return std::ranges::count(controllerButtons,(SDL_GameControllerButton)compareButton) > 0;
        }

        std::vector<SDL_Scancode> keyboardButtons;
        std::vector<SDL_GameControllerButton> controllerButtons;
    };

    inline static const std::map<std::string, InputAction> INPUT_ACTION
    {
        {"moveLeft",{{SDL_SCANCODE_A},{SDL_CONTROLLER_BUTTON_DPAD_LEFT}}},
        {"moveRight",{{SDL_SCANCODE_D},{SDL_CONTROLLER_BUTTON_DPAD_RIGHT}}},
        {"moveDown",{{SDL_SCANCODE_S},{SDL_CONTROLLER_BUTTON_DPAD_DOWN}}},
        {"moveUp",{{SDL_SCANCODE_W},{SDL_CONTROLLER_BUTTON_DPAD_UP}}},


        {"moveLeftController", {{},{SDL_CONTROLLER_BUTTON_DPAD_LEFT}}},
        {"moveRightController",{{},{SDL_CONTROLLER_BUTTON_DPAD_RIGHT}}},
        {"moveDownController", {{},{SDL_CONTROLLER_BUTTON_DPAD_DOWN}}},
        {"moveUpController",   {{},{SDL_CONTROLLER_BUTTON_DPAD_UP}}},

        {"moveKeyboardLeft", {{SDL_SCANCODE_A,SDL_SCANCODE_LEFT},{}}},
        {"moveKeyboardRight",{{SDL_SCANCODE_D,SDL_SCANCODE_RIGHT},{}}},
        {"moveKeyboardDown", {{SDL_SCANCODE_S,SDL_SCANCODE_DOWN},{}}},
        {"moveKeyboardUp",   {{SDL_SCANCODE_W,SDL_SCANCODE_UP},{}}}
    };

    enum class ButtonState
    {
        Down,
        Up,
        Held
    };

    struct InputBinding
    {
        std::unique_ptr<BaseCommand> command;
        InputAction acton;
        ButtonState buttonState;
    };

    class Input final : public Singleton<Input>
	{
    public:
        bool ProcessInput();


        template <typename CommandType, typename... Args>
            requires std::derived_from<CommandType, BaseCommand>
        void RegisterCommand(std::string actionName, ButtonState buttonState, Args&&... args)
        {
            m_Binds.emplace_back(std::make_unique<CommandType>(args...),INPUT_ACTION.at(actionName),buttonState);
        }


        // template <typename CommandType, typename... Args>
        //     requires std::derived_from<CommandType, BaseCommand>
        // void RegisterCommand(SDL_Scancode button, ButtonState buttonState, Args&&... args)
        // {
        //     m_Binds.emplace_back(std::make_unique<CommandType>(args...),button,buttonState);
        // }


	private:
        std::vector<InputBinding> m_Binds;
        SDL_GameController* m_ControllerPtr = nullptr;
	};
}
