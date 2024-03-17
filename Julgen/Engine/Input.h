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
        std::vector<SDL_Scancode> keyboardButtons;
        std::vector<SDL_GameControllerButton> controllerButtons;

        bool HasKeyboardKey(SDL_Scancode compareKey) const
        {
            return std::ranges::count(keyboardButtons,compareKey) > 0;
        }

        bool HasControllerButton(SDL_GameControllerButton compareButton) const
        {
            return std::ranges::count(controllerButtons,compareButton) > 0;
        }
    };

    // TODO: This is currently hardcoded but should ideally be an interface,
    // Or the user of the engine should be able to set dynamically
    inline static const std::map<std::string, InputAction> INPUT_ACTIONS
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
        {"moveKeyboardUp",   {{SDL_SCANCODE_W,SDL_SCANCODE_UP},{}}},

        {"jump",   {{SDL_SCANCODE_SPACE},{SDL_CONTROLLER_BUTTON_A}}},
        {"jumpController",   {{},{SDL_CONTROLLER_BUTTON_A}}},
        {"jumpKeyboard",   {{SDL_SCANCODE_SPACE},{}}}
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

        bool TryExecuteController(ButtonState checkButtonState, int checkControllerIndex,SDL_GameControllerButton compareButton) const
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

        bool TryExecuteKeyboard(ButtonState checkButtonState, SDL_Scancode compareKey) const
        {
            if(buttonState != checkButtonState)
                return false;

            if(not acton.HasKeyboardKey(compareKey))
                return false;

            command->Execute();
            return true;
        }
    };

    class Input final : public Singleton<Input>
	{
    public:

        Input();
        ~Input();

        void ProcessInput(bool& shouldQuit);

        template <typename CommandType, typename... Args>
            requires std::derived_from<CommandType, BaseCommand>
        void RegisterCommand(std::string actionName, ButtonState buttonState,int controllerIndex, Args&&... args)
        {
            m_Binds.emplace_back(buttonState,controllerIndex,INPUT_ACTIONS.at(actionName),std::make_unique<CommandType>(args...));
        }


        template <typename CommandType, typename... Args>
            requires std::derived_from<CommandType, BaseCommand>
        void RegisterCommand(std::string actionName, ButtonState buttonState, Args&&... args)
        {
            m_Binds.emplace_back(buttonState,0,INPUT_ACTIONS.at(actionName),std::make_unique<CommandType>(args...));
        }


	private:

        class ControllerInputImpl;
        std::unique_ptr<ControllerInputImpl> m_ImplUPtr;

        void HandleKeyboardContinually() const;
        // Defined by ControllerInputImpl
        void HandleControllerContinually(const std::vector<InputBinding>& binds);

        [[nodiscard]] bool HandleKeyboardEvent(const SDL_Event& event) const;
        // Defined by ControllerInputImpl
        [[nodiscard]] bool HandleControllerEvent(const SDL_Event& event, const std::vector<InputBinding>& binds);

        std::vector<InputBinding> m_Binds;
	};
}
