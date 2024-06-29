#ifndef INPUTCONTEXT_H
#define INPUTCONTEXT_H

#include <glm/vec2.hpp>
#include <optional>
#include <variant>

namespace jul
{
    enum class ButtonState
    {
        Down,
        Up
    };

    struct InputContext
    {
        ButtonState state;
        std::optional<std::variant<float, glm::vec2>> value;
    };
}  // namespace jul
#endif // INPUTCONTEXT_H