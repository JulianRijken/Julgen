#include "Rigidbody.h"

#include "Physics.h"
#include "Transform.h"

Rigidbody::Rigidbody(jul::GameObject* parent) :
    jul::Component(parent)
{

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f, 4.0f);
    m_BodyPtr = Physics::GetInstance().GetWorld().CreateBody(&bodyDef);


    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 1.1f;
    m_BodyPtr->CreateFixture(&fixtureDef);
}

void Rigidbody::FixedUpdate()
{
    // TODO: This now happends after the internal fixed update
    //       should not just overwrite this is for testing
    auto position = m_BodyPtr->GetPosition();
    glm::vec3 targetPosition = { position.x, position.y, 0 };
    Transform().SetWorldPosition(targetPosition);
}
