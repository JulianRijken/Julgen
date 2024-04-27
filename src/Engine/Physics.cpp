#include "Physics.h"

#include <memory>

#include "GameTime.h"
#include "Rigidbody.h"
#include "Transform.h"

jul::Physics::Physics() :
    m_World(std::make_unique<b2World>(b2Vec2{ 0, -9.81f }))
{
}

void jul::Physics::FixedUpdate()
{
    m_World->Step(jul::GameTime::GetFixedDeltaTimeF(), m_VelocityIterations, m_PositionIterations);
}


void jul::Physics::AddRidgidbody(Rigidbody* rigidbodyPtr)
{
    b2BodyDef bodyDef;  // Define body for Rigidbody based on settings
    {
        const Rigidbody::Settings& settings = rigidbodyPtr->GetSettings();

        // Make sure positons is set correct on init
        const glm::vec3 transformPosition = rigidbodyPtr->Transform().WorldPosition();
        bodyDef.position.Set(transformPosition.x, transformPosition.y);

        const Rigidbody::Mode mode = settings.mode;
        {
            if(mode == Rigidbody::Mode::Dynamic)
                bodyDef.type = b2BodyType::b2_dynamicBody;
            else if(mode == Rigidbody::Mode::Kinematic)
                bodyDef.type = b2BodyType::b2_kinematicBody;
            else if(mode == Rigidbody::Mode::Static)
                bodyDef.type = b2BodyType::b2_staticBody;
        }

        bodyDef.angle = settings.angle;
        bodyDef.linearDamping = settings.linearDamping;
        bodyDef.angularDamping = settings.angularDamping;
        bodyDef.gravityScale = settings.gravityScale;
        bodyDef.allowSleep = settings.allowSleep;
        bodyDef.awake = settings.awake;
        bodyDef.fixedRotation = settings.fixedRotation;
        bodyDef.bullet = settings.avoidTunnelingOnDynamicBodies;
        bodyDef.active = settings.active;
    }

    rigidbodyPtr->m_BodyPtr = m_World->CreateBody(&bodyDef);
}

void jul::Physics::RemoveRidgidbody(Rigidbody* rigidbodyPtr) { m_World->DestroyBody(rigidbodyPtr->m_BodyPtr); }

void jul::Physics::AddCollider(BoxCollider* colliderPtr)
{
    const BoxCollider::Settings& settings = colliderPtr->GetSettings();


    // Collider
    b2PolygonShape boxShape;
    boxShape.SetAsBox(settings.size.x * 0.5f, settings.size.y * 0.5f, { settings.center.x, settings.center.y }, 0.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.density = settings.density;
    fixtureDef.friction = settings.friction;
    fixtureDef.restitution = settings.restitution;

    // If collider gets connected we can just create and return
    if(colliderPtr->m_ConnectedRigidbody)
    {
        colliderPtr->m_ConnectedRigidbody->m_BodyPtr->CreateFixture(&fixtureDef);
        return;
    }


    b2BodyDef bodyDef;
    {
        const glm::vec3 transformPosition = colliderPtr->Transform().WorldPosition();
        bodyDef.position.Set(transformPosition.x, transformPosition.y);

        // TODO: Just the default body gets created for any non connected collider
    }

    // Create body
    colliderPtr->m_BodyPtr = m_World->CreateBody(&bodyDef);

    // Apply fixture
    colliderPtr->m_BodyPtr->CreateFixture(&fixtureDef);
}

void jul::Physics::RemoveCollider(BoxCollider* colliderPtr)
{
    // body can be null when connected
    if(colliderPtr->m_BodyPtr)
        m_World->DestroyBody(colliderPtr->m_BodyPtr);
}
