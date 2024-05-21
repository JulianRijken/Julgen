#include "Command.h"

#include "GameObject.h"
#include "GameTime.h"
#include "Locator.h"
#include "Sound.h"
#include "Sound_Null.h"
#include "Sound_System.h"

jul::MoveCommand::MoveCommand(GameObject* gameObject, float moveSpeed,const glm::vec3& moveDirection) :
      GameObjectCommand(gameObject),
      m_MoveSpeed(moveSpeed),
      m_MoveDirection(moveDirection)
{}

void jul::MoveCommand::Execute(const InputContext& /*context*/)
{
    const glm::vec3 VELOCITY = normalize(m_MoveDirection) * m_MoveSpeed;
    GetGameObject()->GetTransform().Translate(VELOCITY * GameTime::GetDeltaTime<float>());
}


jul::GameObjectCommand::GameObjectCommand(GameObject* gameObject) :
      m_GameObject(gameObject)
{
}


jul::UnitMoveCommand::UnitMoveCommand(GameObject* gameObject,const glm::vec3& moveDirection) :
      GameObjectCommand(gameObject),
      m_MoveDirection(moveDirection)
{}

void jul::UnitMoveCommand::Execute(const InputContext& /*context*/)
{
    GetGameObject()->GetTransform().Translate(m_MoveDirection * 10.0f);
}

void jul::MemberFunctionCommand::Execute(const InputContext& context) { m_Function(context); }

void jul::MuteGameCommand::Execute(const InputContext& /*context*/)
{
    auto* currentSystem = &Locator::Get<Sound>();
    if(dynamic_cast<Sound_Null*>(currentSystem))
    {
        Locator::Release<Sound, Sound_Null>();
        Locator::Provide<Sound, Sound_System>();
    }
    else if(dynamic_cast<Sound_System*>(currentSystem))
    {
        Locator::Release<Sound, Sound_System>();
        Locator::Provide<Sound, Sound_Null>();
    }
}
