#include "Input.h"

namespace jul
{
    class Input::InputImpl
    {

    public:

        void UpdateControllers()
        {
            int jystickCount = SDL_NumJoysticks();

            if (static_cast<int>(m_Controllers.size()) < jystickCount)
                m_Controllers.resize(jystickCount,nullptr);

            for (int i = 0; i < jystickCount; i++)
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


        void HandleController()
        {
            UpdateControllers();

            for (auto&& bind : Input::GetInstance().m_Binds)
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

    private:

        std::vector<SDL_GameController*> m_Controllers;
    };
}

void jul::Input::HandleController()
{
    m_ImplUPtr->HandleController();
}

jul::Input::Input() :
      m_ImplUPtr(std::make_unique<InputImpl>())
{};

jul::Input::~Input() = default;

