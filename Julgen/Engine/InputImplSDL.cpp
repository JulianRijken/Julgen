#include "Input.h"

namespace jul
{
    class Input::InputImpl
    {

    public:

        SDL_GameController* FindController()
        {
            for (int i = 0; i < SDL_NumJoysticks(); i++)
            {
                if (SDL_IsGameController(i))
                    return SDL_GameControllerOpen(i);
            }

            return nullptr;
        }

        void HandleController()
        {
            if(m_ControllerPtr == nullptr)
                m_ControllerPtr = FindController();

            for (auto&& bind :   Input::GetInstance().m_Binds)
            {
                for (auto&& controllerButton : bind.acton.controllerButtons)
                {
                    if(bind.buttonState == ButtonState::Held and SDL_GameControllerGetButton(m_ControllerPtr,controllerButton))
                    {
                        bind.command->Execute();
                        break;
                    }
                }
            }
        }

    private:

        SDL_GameController* m_ControllerPtr = nullptr;

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

