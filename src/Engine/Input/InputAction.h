#ifndef INPUTACTION_H
#define INPUTACTION_H
#include <SDL_gamecontroller.h>
#include <SDL_scancode.h>

#include <algorithm>
#include <vector>

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
}  // namespace jul
#endif // INPUTACTION_H