#include "Input.h"

namespace jul
{
class Input::ControllerInputImpl
    {

    public:

        void HandleControllerContinually(const std::vector<InputBinding>& binds)
        {
            UpdateControllers();

            for (auto&& bind : binds)
            {
                // Get the controller and check if it exists
                if (SDL_GameController* controller = GetController(bind.controllerIndex))
                {
                    for (auto&& controllerButton : bind.acton.controllerButtons)
                    {
                        if (bind.buttonState == ButtonState::Held and SDL_GameControllerGetButton(controller,controllerButton))
                        {
                            bind.command->Execute();
                            break;
                        }
                    }
                }
            }
        }


        bool HandleControllerEvent(const SDL_Event& event,const std::vector<InputBinding>& binds)
        {
            switch (event.type)
            {
            case SDL_CONTROLLERBUTTONDOWN:
                for (auto&& bind : binds)
                    bind.TryExcecuteController(ButtonState::Down,event.cbutton.which,static_cast<SDL_GameControllerButton>(event.cbutton.button));
                return true;

            case SDL_CONTROLLERBUTTONUP:
                for (auto&& bind : binds)
                    bind.TryExcecuteController(ButtonState::Up,event.cbutton.which,static_cast<SDL_GameControllerButton>(event.cbutton.button));
                return true;
            }

            return false;
        }

    private:


        void UpdateControllers()
        {
            int joystickCount = SDL_NumJoysticks();

            if (static_cast<int>(m_Controllers.size()) < joystickCount)
                m_Controllers.resize(joystickCount,nullptr);

            for (int i = 0; i < joystickCount; i++)
            {
                if (not SDL_IsGameController(i))
                    m_Controllers[i] = nullptr;

                if(m_Controllers[i] != nullptr)
                    continue;

                m_Controllers[i] = SDL_GameControllerOpen(i);
            }
        }


        SDL_GameController* GetController(int controllerIndex)
        {
            // Check if controller is out of bounds
            if(controllerIndex >= static_cast<int>(m_Controllers.size()))
                return nullptr;

            return m_Controllers[controllerIndex];
        }


        std::vector<SDL_GameController*> m_Controllers;
    };
}

void jul::Input::HandleControllerContinually(const std::vector<InputBinding>& binds)
{
    m_ImplUPtr->HandleControllerContinually(binds);
}

bool jul::Input::HandleControllerEvent(const SDL_Event& event, const std::vector<InputBinding>& binds)
{
    return m_ImplUPtr->HandleControllerEvent(event,binds);
}

jul::Input::Input() :
      m_ImplUPtr(std::make_unique<ControllerInputImpl>())
{};

jul::Input::~Input() = default;

