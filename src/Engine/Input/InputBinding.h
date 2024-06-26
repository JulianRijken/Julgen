#ifndef INPUTBINDING_H
#define INPUTBINDING_H
#include <Command.h>

#include <memory>

#include "InputAction.h"


namespace jul
{
    struct InputBinding
    {
        int controllerIndex;
        bool allowKeyboard;
        InputAction acton;
        std::unique_ptr<BaseCommand> command;

        bool TryExecuteController(ButtonState buttonState, int checkControllerIndex,
                                  SDL_GameControllerButton compareButton) const
        {
            if(controllerIndex != checkControllerIndex)
                return false;

            if(not acton.HasControllerButton(compareButton))
                return false;

            command->Execute({ buttonState, {} });
            return true;
        }

        bool TryExecuteKeyboard(ButtonState buttonState, SDL_Scancode compareKey) const
        {
            if(not acton.HasKeyboardKey(compareKey))
                return false;

            command->Execute({ buttonState, {} });
            return true;
        }
    };
}  // namespace jul
#endif // INPUTBINDING_H