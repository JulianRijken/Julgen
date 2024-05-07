#include "Rigidbody.h"

#include <fmt/core.h>

#include "GameObject.h"
#include "ICollisionListener.h"
#include "Locator.h"
#include "Physics.h"
#include "Transform.h"

jul::Rigidbody::Rigidbody(GameObject* parentPtr, Settings setting) :
    Component(parentPtr, "Rigidbody"),
    m_Settings(setting)
{
    Locator::Get<Physics>().AddRidgidbody(this);
}

jul::Rigidbody::~Rigidbody() { Locator::Get<Physics>().RemoveRidgidbody(this); }

glm::vec2 jul::Rigidbody::Positon()
{
    auto [x, y] = m_BodyPtr->GetPosition();
    return { x, y };
}

glm::vec2 jul::Rigidbody::Velocity()
{
    auto [x, y] = m_BodyPtr->GetLinearVelocity();
    return { x, y };
}

glm::vec2 jul::Rigidbody::LinearVelocityFromWorldPoint(glm::vec2 worldPoint)
{
    const b2Vec2 velocity = m_BodyPtr->GetLinearVelocityFromWorldPoint({ worldPoint.x, worldPoint.y });
    return { velocity.x, velocity.y };
}

void jul::Rigidbody::SetGravityScale(float scale) { m_BodyPtr->SetGravityScale(scale); }

void jul::Rigidbody::AddForce(glm::vec2 force, ForceMode forceMode, bool wake)
{
    const b2Vec2 b2Force = { force.x, force.y };

    if(forceMode == ForceMode::Force)
        m_BodyPtr->ApplyForceToCenter(b2Force, wake);
    else if(forceMode == ForceMode::Impulse)
        m_BodyPtr->ApplyLinearImpulseToCenter(b2Force, wake);
    if(forceMode == ForceMode::VelocityChange)
        m_BodyPtr->SetLinearVelocity(b2Force);
}

void jul::Rigidbody::SetPosition(glm::vec2 position) { m_BodyPtr->SetTransform({ position.x, position.y }, 0.0f); }


void jul::Rigidbody::FixedUpdate()
{
    // TODO: This now happends after the internal fixed update
    //       should not just overwrite this is for testing
    auto position = m_BodyPtr->GetPosition();
    const glm::vec3 targetPosition = { position.x, position.y, 0 };
    GetTransform().SetWorldPosition(targetPosition);


    // TODO: Use dirty flag/caching to avoid this call every collision!
    // THIS IS HORRIBLE AND SLOW CODE PLEASE FIX THIS AS FAST AS POSSIBLE!
    // If this is still here, there was no time :(
    std::vector<Component*> components = GetGameObject()->GetComponents<Component>();
    m_CollisionListeners.clear();
    for(Component* component : components)
    {
        ICollisionListener* listener = dynamic_cast<ICollisionListener*>(component);
        if(listener)
            m_CollisionListeners.push_back(listener);
    }
}

void jul::Rigidbody::OnCollisionBegin(Collision collision)
{
    for(auto&& listener : m_CollisionListeners)
        listener->OnCollisionBegin(collision);
}

void jul::Rigidbody::OnCollisionEnd(Collision collision)
{
    for(auto&& listener : m_CollisionListeners)
        listener->OnCollisionEnd(collision);
}

void jul::Rigidbody::OnCollisionPreSolve(Collision collision, const b2Manifold* oldManifold)
{
    for(auto&& listener : m_CollisionListeners)
        listener->OnCollisionPreSolve(collision, oldManifold);
}

void jul::Rigidbody::OnCollisionPostSolve(Collision collision)
{
    for(auto&& listener : m_CollisionListeners)
        listener->OnCollisionPostSolve(collision);
}
