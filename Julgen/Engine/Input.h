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
    inline static const float STICK_DEADZONE{0.15f};
    inline static const float TRIGGER_DEADZONE{0.05f};


    struct InputAction
    {
        std::vector<SDL_Scancode> keyboardButtons;
        std::vector<SDL_GameControllerButton> controllerButtons;
        std::vector<SDL_GameControllerAxis> controllerAxis;

        bool HasKeyboardKey(SDL_Scancode compareKey) const
        {
            return std::ranges::count(keyboardButtons,compareKey) > 0;
        }

        bool HasControllerButton(SDL_GameControllerButton compareButton) const
        {
            return std::ranges::count(controllerButtons,compareButton) > 0;
        }

        bool HasControllerAxis(SDL_GameControllerAxis compareAxis) const
        {
            return std::ranges::count(controllerAxis,compareAxis) > 0;
        }
    };


    enum class InputBind
    {
        TestLivesButton,
        Jump,
        MoveLeft,
        MoveRight,
        MoveStick,
        Attack,
    };

    inline static const std::map<InputBind, InputAction> INPUT_ACTIONS
    {
        // {"moveLeft",{{SDL_SCANCODE_A},{SDL_CONTROLLER_BUTTON_DPAD_LEFT},{}}},
        // {"moveRight",{{SDL_SCANCODE_D},{SDL_CONTROLLER_BUTTON_DPAD_RIGHT},{}}},
        // {"moveDown",{{SDL_SCANCODE_S},{SDL_CONTROLLER_BUTTON_DPAD_DOWN},{}}},
        // {"moveUp",{{SDL_SCANCODE_W},{SDL_CONTROLLER_BUTTON_DPAD_UP},{}}},


        // {"moveLeftController", {{},{SDL_CONTROLLER_BUTTON_DPAD_LEFT},{}}},
        // {"moveRightController",{{},{SDL_CONTROLLER_BUTTON_DPAD_RIGHT},{}}},
        // {"moveDownController", {{},{SDL_CONTROLLER_BUTTON_DPAD_DOWN},{}}},
        // {"moveUpController",   {{},{SDL_CONTROLLER_BUTTON_DPAD_UP},{}}},

        // {"moveKeyboardLeft", {{SDL_SCANCODE_A,SDL_SCANCODE_LEFT},{},{}}},
        // {"moveKeyboardRight",{{SDL_SCANCODE_D,SDL_SCANCODE_RIGHT},{},{}}},
        // {"moveKeyboardDown", {{SDL_SCANCODE_S,SDL_SCANCODE_DOWN},{},{}}},
        // {"moveKeyboardUp",   {{SDL_SCANCODE_W,SDL_SCANCODE_UP},{},{}}},

        // {"jump",             {{SDL_SCANCODE_SPACE},{SDL_CONTROLLER_BUTTON_A},{}}},
        // {"jumpController",   {{},{SDL_CONTROLLER_BUTTON_A},{}}},
        // {"jumpKeyboard",     {{SDL_SCANCODE_SPACE},{},{}}},

        // {"stickExample",   {{},{},{SDL_CONTROLLER_AXIS_RIGHTY,SDL_CONTROLLER_AXIS_RIGHTX}}},
        // {"triggerExample",   {{},{},{SDL_CONTROLLER_AXIS_TRIGGERRIGHT}}},

        {InputBind::TestLivesButton,{{SDL_SCANCODE_X},{SDL_CONTROLLER_BUTTON_B},{}}},
        {InputBind::Jump,{{SDL_SCANCODE_SPACE},{SDL_CONTROLLER_BUTTON_B},{}}},
        {InputBind::MoveLeft,{{SDL_SCANCODE_A},{SDL_CONTROLLER_BUTTON_DPAD_LEFT},{}}},
        {InputBind::MoveRight,{{SDL_SCANCODE_D},{SDL_CONTROLLER_BUTTON_DPAD_RIGHT},{}}},
        {InputBind::MoveStick,{{},{},{SDL_CONTROLLER_AXIS_LEFTX}}},
        {InputBind::Attack,{{SDL_SCANCODE_E,SDL_SCANCODE_Z},{SDL_CONTROLLER_BUTTON_A},{}}},
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
        bool allowKeyboard;
        InputAction acton;
        std::unique_ptr<BaseCommand> command;

        bool TryExecuteController(ButtonState checkButtonState, int checkControllerIndex,SDL_GameControllerButton compareButton) const;
        bool TryExecuteKeyboard(ButtonState checkButtonState, SDL_Scancode compareKey) const;
    };

    class Input final : public Singleton<Input>
	{
    public:

        ~Input();
        Input();

        template<typename DataType>
        static float NormalizeAxis(const DataType& rawAxis, float deadzone, std::optional<float> axisLimit = std::nullopt)
        {
            // Oh my god this axisLimit.value_or is the sickes syntax I have ever seen!
            float input(static_cast<float>(rawAxis) / static_cast<float>(axisLimit.value_or(std::numeric_limits<DataType>::max())));
            input = std::clamp(input,-1.0f,1.0f);
            if(std::abs(input) < deadzone)
                input = 0;

            return input;
        }


        void ProcessInput(bool& shouldQuit);

        template <typename CommandType, typename... Args>
            requires std::derived_from<CommandType, BaseCommand>
        void RegisterCommand(InputBind actionName, ButtonState buttonState,int controllerIndex,bool allowKeyboard, Args&&... args)
        {
            assert(INPUT_ACTIONS.contains(actionName) && "Action Does Not Exist");
            m_Binds.emplace_back(buttonState,controllerIndex,allowKeyboard,INPUT_ACTIONS.at(actionName),std::make_unique<CommandType>(args...));
        }


        class ControllerInputImpl;
        std::unique_ptr<ControllerInputImpl> m_ImplUPtr;

        void HandleKeyboardContinually() const;
        // Defined by ControllerInputImpl
        void HandleControllerContinually();

        [[nodiscard]] bool HandleKeyboardEvent(const SDL_Event& event) const;
        // Defined by ControllerInputImpl
        [[nodiscard]] bool HandleControllerEvent(const SDL_Event& event);

        std::vector<InputBinding> m_Binds;
	};
}
