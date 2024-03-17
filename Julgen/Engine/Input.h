#pragma once
#include <SDL.h>
#include <SDL_keycode.h>
#include <algorithm>
#include <map>
#include <memory>
#include <string>
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

        bool HasControllerButton(SDL_GameControllerButton compareButton)
        {
            return std::ranges::count(controllerButtons,compareButton) > 0;
        }

        std::vector<SDL_Scancode> keyboardButtons;
        std::vector<SDL_GameControllerButton> controllerButtons;
    };

    // TODO: This is currenty hardcoded but should idealy be a interface,
    // Or the user of the engine should be able to set dynamically
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
        ButtonState buttonState;
        int controllerIndex;
        InputAction acton;
        std::unique_ptr<BaseCommand> command;
    };

    class Input final : public Singleton<Input>
	{
    public:

        Input();
        ~Input();

        bool ProcessInput();

        template <typename CommandType, typename... Args>
            requires std::derived_from<CommandType, BaseCommand>
        void RegisterCommand(std::string actionName, ButtonState buttonState,int controllerIndex, Args&&... args)
        {
            m_Binds.emplace_back(buttonState,controllerIndex,INPUT_ACTION.at(actionName),std::make_unique<CommandType>(args...));
        }


        template <typename CommandType, typename... Args>
            requires std::derived_from<CommandType, BaseCommand>
        void RegisterCommand(std::string actionName, ButtonState buttonState, Args&&... args)
        {
            m_Binds.emplace_back(buttonState,0,INPUT_ACTION.at(actionName),std::make_unique<CommandType>(args...));
        }


	private:

        class InputImpl;
        friend class InputImpl;
        std::unique_ptr<InputImpl> m_ImplUPtr;

        void HandleController();

        std::vector<InputBinding> m_Binds;
	};
}
