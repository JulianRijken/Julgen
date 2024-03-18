#include "Command.h"
#include "GameObject.h"
#include "GameTime.h"

jul::MoveCommand::MoveCommand(GameObject* gameObject, float moveSpeed, glm::vec3 moveDirection) :
      GameObjectCommand(gameObject),
      m_MoveSpeed(moveSpeed),
      m_MoveDirection(moveDirection)
{}

void jul::MoveCommand::Execute(InputContext)
{
    glm::vec3 velocity = glm::normalize(m_MoveDirection) * m_MoveSpeed;
    GetGameObject()->GetTransform().Translate(velocity * GameTime::GetDeltaTimeF());
}


jul::GameObjectCommand::GameObjectCommand(GameObject* gameObject) :
      m_GameObject(gameObject)
{
}


jul::UnitMoveCommand::UnitMoveCommand(GameObject* gameObject, glm::vec3 moveDirection) :
      GameObjectCommand(gameObject),
      m_MoveDirection(moveDirection)
{}

void jul::UnitMoveCommand::Execute(InputContext)
{
    GetGameObject()->GetTransform().Translate(m_MoveDirection * 10.0f);
}

void jul::StickTestCommand::Execute(InputContext context)
{
    auto input = std::get<glm::vec2>(context.value());
    std::cout <<  "Stick Input X: " << input.x << std::endl;
    std::cout <<  "Stick Input Y: " << input.y << std::endl;
}

void jul::TriggerTestCommand::Execute(InputContext context)
{
    auto input = std::get<float>(context.value());
    std::cout << "Trigger Input: " << input << std::endl;
}


