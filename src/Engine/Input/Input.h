#pragma once

#include <SDL.h>
#include <SDL_keycode.h>

#include <algorithm>
#include <glm/vec2.hpp>
#include <map>
#include <memory>
#include <optional>
#include <vector>

#include "Command.h"
#include "Event.h"
#include "InputAction.h"
#include "InputBinding.h"
#include "Singleton.h"

namespace jul
{
    class Input final : public Singleton<Input>
    {
    public:
        inline static constexpr float STICK_DEADZONE{ 0.15f };
        inline static constexpr float TRIGGER_DEADZONE{ 0.05f };
        inline static constexpr float AXIS_LIMIT{ std::numeric_limits<short>::max() };

        // Exclusivly called by engine
        void ProcessInput(bool& shouldQuit);

        // Used as a shorthand for a function command
        template<typename... Args, typename ActionEnum>
        static void Bind(ActionEnum actionEnum, int controllerIndex, bool allowKeyboard, Args&&... args)
        {
            GetInstance().RegisterCommand<EventCommand>(
                static_cast<int>(actionEnum), controllerIndex, allowKeyboard, std::forward<Args>(args)...);
        }

        template<typename ActionEnum>
        static void AddAction(ActionEnum actionEnum, InputAction&& action)
        {
            GetInstance().m_InputActions.emplace(static_cast<int>(actionEnum), std::move(action));
        }

        template<typename CommandType, typename... Args>
            requires std::derived_from<CommandType, BaseCommand>
        static void RegisterCommand(int actionName, int controllerIndex, bool allowKeyboard, Args&&... args)
        {
            assert(GetInstance().m_InputActions.contains(actionName) && "Action Does Not Exist");

            GetInstance().m_Binds.emplace_back(controllerIndex,
                                               allowKeyboard,
                                               GetInstance().m_InputActions.at(actionName),
                                               std::make_unique<CommandType>(std::forward<Args>(args)...));
        }

    private:
        std::vector<InputBinding> m_Binds{};
        std::map<int, InputAction> m_InputActions{};
        std::vector<SDL_GameController*> m_Controllers;

        [[nodiscard]] bool HandleKeyboardEvent(const SDL_Event& event) const;
        [[nodiscard]] bool HandleControllerEvent(const SDL_Event& event) const;
        [[nodiscard]] SDL_GameController* GetController(int controllerIndex) const;

        void HandleControllerAxis(const SDL_GameControllerAxis& axis) const;
        void UpdateControllers();

        template<typename DataType>
        static float NormalizeAxis(const DataType& rawAxis, float deadzone,
                                   std::optional<float> axisLimit = std::nullopt)
        {
            float input(static_cast<float>(rawAxis) /
                        static_cast<float>(axisLimit.value_or(std::numeric_limits<DataType>::max())));
            input = std::clamp(input, -1.0f, 1.0f);
            if(std::abs(input) < deadzone)
                input = 0;

            return input;
        }
    };
}
