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
        std::vector<SDL_Scancode> keyboardButtons;
        std::vector<SDL_GameControllerButton> controllerButtons;
        std::vector<SDL_GameControllerAxis> controllerAxis;

        [[nodiscard]] bool HasKeyboardKey(SDL_Scancode compareKey) const
        {
            return std::ranges::count(keyboardButtons, compareKey) > 0;
        }

        [[nodiscard]] bool HasControllerButton(SDL_GameControllerButton compareButton) const
        {
            return std::ranges::count(controllerButtons, compareButton) > 0;
        }

        [[nodiscard]] bool HasControllerAxis(SDL_GameControllerAxis compareAxis) const
        {
            return std::ranges::count(controllerAxis, compareAxis) > 0;
        }
    };

    enum class ButtonState
    {
        Down,
        Up,
        Held
    };

    struct InputBinding
    {
        ButtonState state;
        int controllerIndex;
        bool allowKeyboard;
        InputAction acton;
        std::unique_ptr<BaseCommand> command;

        bool TryExecuteController(ButtonState checkButtonState, int checkControllerIndex,
                                  SDL_GameControllerButton compareButton) const;
        bool TryExecuteKeyboard(ButtonState checkButtonState, SDL_Scancode compareKey) const;
    };

    class Input final : public Singleton<Input>
    {

    public:
        Input();
        ~Input() override;

        Input(const Input&) = delete;
        Input(Input&&) noexcept = delete;
        Input& operator=(const Input&) = delete;
        Input& operator=(Input&&) noexcept = delete;

        // Used as a shorthand for a function command
        template<typename... Args, typename ActionEnum>
        static void Bind(ActionEnum actionEnum, ButtonState buttonState, int controllerIndex, bool allowKeyboard,
                         Args&&... args)
        {
            GetInstance().RegisterCommand<MemberFunctionCommand>(
                static_cast<int>(actionEnum), buttonState, controllerIndex, allowKeyboard, args...);
        }

        template<typename ActionEnum>
        static void AddAction(ActionEnum actionEnum, InputAction&& action)
        {
            GetInstance().m_InputActions.emplace(static_cast<int>(actionEnum), action);
        }

        inline static float g_StickDeadzone{ 0.15f };
        inline static float g_TriggerDeadzone{ 0.05f };


        void ProcessInput(bool& shouldQuit);

        template<typename CommandType, typename... Args>
            requires std::derived_from<CommandType, BaseCommand>
        void RegisterCommand(int actionName, ButtonState buttonState, int controllerIndex, bool allowKeyboard,
                             Args&&... args)
        {
            assert(m_InputActions.contains(actionName) && "Action Does Not Exist");
            m_Binds.emplace_back(buttonState,
                                 controllerIndex,
                                 allowKeyboard,
                                 m_InputActions.at(actionName),
                                 std::make_unique<CommandType>(args...));
        }

    private:
        class ControllerInputImpl;
        std::unique_ptr<ControllerInputImpl> m_ImplUPtr;

        template<typename DataType>
        static float NormalizeAxis(const DataType& rawAxis, float deadzone,
                                   std::optional<float> axisLimit = std::nullopt)
        {
            // Oh my god this axisLimit.value_or is the sickes syntax I have ever seen!
            float input(static_cast<float>(rawAxis) /
                        static_cast<float>(axisLimit.value_or(std::numeric_limits<DataType>::max())));
            input = std::clamp(input, -1.0f, 1.0f);
            if(std::abs(input) < deadzone)
                input = 0;

            return input;
        }

        void HandleKeyboardContinually() const;
        // Defined by ControllerInputImpl
        void HandleControllerContinually();

        [[nodiscard]] bool HandleKeyboardEvent(const SDL_Event& event) const;
        // Defined by ControllerInputImpl
        [[nodiscard]] bool HandleControllerEvent(const SDL_Event& event);

        std::vector<InputBinding> m_Binds;
        std::map<int, InputAction> m_InputActions{};
    };
}
