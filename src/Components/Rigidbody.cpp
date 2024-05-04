#include "Rigidbody.h"

#include <fmt/core.h>

#include "Locator.h"
#include "Physics.h"
#include "Transform.h"

jul::Rigidbody::Rigidbody(GameObject* parentPtr, Settings setting) :
    Component(parentPtr),
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

void jul::Rigidbody::AddForce(glm::vec2 force, ForceMode forceMode, bool wake)
{
    const b2Vec2 b2Force = { force.x, force.y };

    if(forceMode == ForceMode::Force)
        m_BodyPtr->ApplyForceToCenter(b2Force, wake);
    else if(forceMode == ForceMode::Impulse)
        m_BodyPtr->ApplyLinearImpulseToCenter(b2Force, wake);
}

void jul::Rigidbody::SetPosition(glm::vec2 position) { m_BodyPtr->SetTransform({ position.x, position.y }, 0.0f); }

void jul::Rigidbody::FixedUpdate()
{
    // TODO: This now happends after the internal fixed update
    //       should not just overwrite this is for testing
    auto position = m_BodyPtr->GetPosition();
    const glm::vec3 targetPosition = { position.x, position.y, 0 };
    GetTransform().SetWorldPosition(targetPosition);
}

void jul::Rigidbody::BeginContact(b2Contact* contact)
{
    if(contact->GetFixtureA()->GetBody() == m_BodyPtr)
        fmt::println("Contact Begin");
}

void jul::Rigidbody::EndContact(b2Contact*) { fmt::println("Contact End"); }
