#include "Rigidbody.h"

#include "Locator.h"
#include "Physics.h"
#include "Transform.h"

jul::Rigidbody::Rigidbody(jul::GameObject* parent) :
    jul::Component(parent)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;

    // Init the start positon
    const glm::vec3 transformPosition = Transform().WorldPosition();
    bodyDef.position.Set(transformPosition.x, transformPosition.y);

    bodyDef.fixedRotation = true;  // TODO: Game currently does not yet support rotation
    m_BodyPtr = Locator::Get<Physics>().GetWorld().CreateBody(&bodyDef);


    // Collider
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 1.0f;

    m_BodyPtr->CreateFixture(&fixtureDef);
}

glm::vec2 jul::Rigidbody::Positon()
{
    auto [x, y] = m_BodyPtr->GetPosition();
    return { x, y };
}

glm::vec2 jul::Rigidbody::Velicty()
{
    auto [x, y] = m_BodyPtr->GetLinearVelocity();
    return { x, y };
}

void jul::Rigidbody::AddForce(glm::vec2 force, ForceMode forceMode, bool wake)
{
    b2Vec2 b2Force = { force.x, force.y };

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
    Transform().SetWorldPosition(targetPosition);
}
