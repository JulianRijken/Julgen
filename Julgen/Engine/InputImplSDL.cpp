#include "Input.h"

namespace jul
{
class Input::ControllerInputImpl
{

    const float AXIS_LIMIT{std::numeric_limits<short>::max()};

    public:

        void HandleControllerContinually(const std::vector<InputBinding>& binds)
        {
            UpdateControllers();

            for (auto&& bind : binds)
            {
                if (bind.state != ButtonState::Held)
                    continue;

                // Get the controller and check if it exists
                if (SDL_GameController* controller = GetController(bind.controllerIndex))
                {
                    // Handle all controller buttons
                    for (auto&& controllerButton : bind.acton.controllerButtons)
                    {
                        if(SDL_GameControllerGetButton(controller,controllerButton))
                        {
                            bind.command->Execute();
                            break;
                        }
                    }

                    // Handle right stick
                    if(bind.acton.HasControllerAxis(SDL_CONTROLLER_AXIS_RIGHTX) and bind.acton.HasControllerAxis(SDL_CONTROLLER_AXIS_RIGHTY))
                    {
                        bind.command->Execute(glm::vec2{
                        Input::NormalizeAxis(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX),STICK_DEADZONE,AXIS_LIMIT),
                        -Input::NormalizeAxis(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY),STICK_DEADZONE,AXIS_LIMIT)});
                        continue;
                    }

                    // Handle left stick
                    if(bind.acton.HasControllerAxis(SDL_CONTROLLER_AXIS_LEFTX) and bind.acton.HasControllerAxis(SDL_CONTROLLER_AXIS_LEFTY))
                    {
                        bind.command->Execute(glm::vec2{
                        Input::NormalizeAxis(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX),STICK_DEADZONE,AXIS_LIMIT),
                        -Input::NormalizeAxis(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY),STICK_DEADZONE,AXIS_LIMIT)});
                        continue;
                    }

                    // Handle all remaining axis
                    for (auto&& controllerAxis : bind.acton.controllerAxis)
                        bind.command->Execute(Input::NormalizeAxis(SDL_GameControllerGetAxis(controller,controllerAxis),STICK_DEADZONE,AXIS_LIMIT));
                }
            }
        }


        bool HandleControllerEvent(const SDL_Event& event,const std::vector<InputBinding>& binds)
        {
            switch (event.type)
            {
            case SDL_CONTROLLERBUTTONDOWN:
            case SDL_CONTROLLERBUTTONUP:
	            const ButtonState buttonState = event.type == SDL_CONTROLLERBUTTONDOWN ? ButtonState::Down : ButtonState::Up;

                for (auto&& bind : binds)
                    bind.TryExecuteController(buttonState,event.cbutton.which,static_cast<SDL_GameControllerButton>(event.cbutton.button));

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

void jul::Input::HandleControllerContinually()
{
    m_ImplUPtr->HandleControllerContinually(m_Binds);
}

bool jul::Input::HandleControllerEvent(const SDL_Event& event)
{
    return m_ImplUPtr->HandleControllerEvent(event,m_Binds);
}

jul::Input::Input() :
      m_ImplUPtr(std::make_unique<ControllerInputImpl>())
{};

jul::Input::~Input() = default;

