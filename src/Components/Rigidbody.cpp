#include "Rigidbody.h"

#include <fmt/core.h>

#include "GameObject.h"
#include "ICollisionListener.h"
#include "Locator.h"
#include "Physics.h"
#include "Transform.h"

jul::Rigidbody::Rigidbody(GameObject* parentPtr, const Settings& settings) :
    Component(parentPtr, "Rigidbody"),
    m_Settings(settings),
    m_ModeFlag(settings.mode)
{
    GetTransform().SetRigidbody(this);
    Locator::Get<Physics>().AddRigidbody(this);
}

jul::Rigidbody::~Rigidbody()
{
    m_CollisionListeners.clear();
    Locator::Get<Physics>().RemoveRigidbody(this);
}

glm::vec2 jul::Rigidbody::Position() const
{
    auto [x, y] = m_BodyPtr->GetPosition();
    return { x, y };
}

glm::vec2 jul::Rigidbody::Velocity() const
{
    auto [x, y] = m_BodyPtr->GetLinearVelocity();
    return { x, y };
}

glm::vec2 jul::Rigidbody::LinearVelocityFromWorldPoint(glm::vec2 worldPoint) const
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

void jul::Rigidbody::SetPosition(glm::vec2 position) const
{
    m_BodyPtr->SetTransform({ position.x, position.y }, 0.0f);
}


void jul::Rigidbody::FixedUpdate()
{
    if(m_ModeFlag != m_Settings.mode)
    {
        m_Settings.mode = m_ModeFlag;

        b2BodyType newType{};
        if(m_Settings.mode == Rigidbody::Mode::Dynamic)
            newType = b2BodyType::b2_dynamicBody;
        else if(m_Settings.mode == Rigidbody::Mode::Kinematic)
            newType = b2BodyType::b2_kinematicBody;
        else if(m_Settings.mode == Rigidbody::Mode::Static)
            newType = b2BodyType::b2_staticBody;

        m_BodyPtr->SetType(newType);
    }

    UpdateCollisionListeners();

    // Static allows it to be moved manually
    if(m_Settings.mode != Mode::Static)
    {
        const auto position = m_BodyPtr->GetPosition();
        const glm::vec3 targetPosition = { position.x, position.y, 0 };
        GetTransform().SetWorldPosition(targetPosition);
    }
}

void jul::Rigidbody::SetMode(Mode mode) { m_ModeFlag = mode; }

void jul::Rigidbody::SetVelocity(glm::vec2 velocity) const { m_BodyPtr->SetLinearVelocity({ velocity.x, velocity.y }); }

void jul::Rigidbody::UpdateCollisionListeners()
{
    // TODO: Use dirty flag/caching to avoid this call every collision!
    // Better yet use binding when adding the collider and removing
    // THIS IS HORRIBLE AND SLOW CODE PLEASE FIX THIS AS FAST AS POSSIBLE!
    // If this is still here, there was no time :(
    const std::vector<Component*> components = GetGameObject()->GetComponents<Component>();
    m_CollisionListeners.clear();
    for(Component* component : components)
    {
        if(auto* listener = dynamic_cast<ICollisionListener*>(component))
            m_CollisionListeners.emplace(listener);
    }
}

// void jul::Rigidbody::AddCollisionListener(ICollisionListener* collisionListenerPtr)
// {
//     m_CollisionListeners.insert(collisionListenerPtr);
// }

// void jul::Rigidbody::RemoveCollisionListener(ICollisionListener* collisionListenerPtr)
// {
//     m_CollisionListeners.erase(collisionListenerPtr);
// }


void jul::Rigidbody::OnCollisionBegin(const Collision& collision) const
{
    for(auto&& listener : m_CollisionListeners)
        listener->OnCollisionBegin(collision);
}

void jul::Rigidbody::OnCollisionEnd(const Collision& collision) const
{
    for(auto&& listener : m_CollisionListeners)
        listener->OnCollisionEnd(collision);
}

void jul::Rigidbody::OnCollisionPreSolve(const Collision& collision, const b2Manifold* oldManifold) const
{
    for(auto&& listener : m_CollisionListeners)
        listener->OnCollisionPreSolve(collision, oldManifold);
}

void jul::Rigidbody::OnCollisionPostSolve(const Collision& collision) const
{
    for(auto&& listener : m_CollisionListeners)
        listener->OnCollisionPostSolve(collision);
}
