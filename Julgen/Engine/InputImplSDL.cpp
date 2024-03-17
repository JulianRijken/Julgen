#include "Input.h"

namespace jul
{
class Input::ControllerInputImpl
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


        void HandleControllerContinually()
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


        bool HandleControllerEvent(const SDL_Event& event)
        {
            switch (event.type)
            {
            case SDL_CONTROLLERBUTTONDOWN:
                for (auto&& bind : Input::GetInstance().m_Binds)
                    bind.TryExcecuteController(ButtonState::Down,event.cbutton.which,static_cast<SDL_GameControllerButton>(event.cbutton.button));
                return true;

            case SDL_CONTROLLERBUTTONUP:
                for (auto&& bind : Input::GetInstance().m_Binds)
                    bind.TryExcecuteController(ButtonState::Up,event.cbutton.which,static_cast<SDL_GameControllerButton>(event.cbutton.button));
                return true;
            }

            return false;
        }

    private:

        std::vector<SDL_GameController*> m_Controllers;
    };
}

void jul::Input::HandleControllerContinually()
{
    m_ImplUPtr->HandleControllerContinually();
}

bool jul::Input::HandleControllerEvent(const SDL_Event& event)
{
    return m_ImplUPtr->HandleControllerEvent(event);
}

jul::Input::Input() :
      m_ImplUPtr(std::make_unique<ControllerInputImpl>())
{};

jul::Input::~Input() = default;

