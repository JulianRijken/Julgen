#include "Physics.h"

#include <memory>

#include "GameTime.h"
#include "Rigidbody.h"

jul::Physics::Physics() :
    m_World(std::make_unique<b2World>(b2Vec2{ 0, -9.81f }))
{
}

void jul::Physics::FixedUpdate()
{
    m_World->Step(jul::GameTime::GetFixedDeltaTimeF(), m_VelocityIterations, m_PositionIterations);
}

void jul::Physics::AddRidgidbody(Rigidbody* rigidbody)
{
    b2BodyDef bodyDef;

    const Rigidbody::Mode mode = rigidbody->m_Settings.mode;
    {
        if(mode == Rigidbody::Mode::Dynamic)
            bodyDef.type = b2BodyType::b2_dynamicBody;
        else if(mode == Rigidbody::Mode::Kinematic)
            bodyDef.type = b2BodyType::b2_kinematicBody;
        else if(mode == Rigidbody::Mode::Static)
            bodyDef.type = b2BodyType::b2_staticBody;
    }

    bodyDef.angle = rigidbody->m_Settings.angle;
    bodyDef.linearDamping = rigidbody->m_Settings.linearDamping;
    bodyDef.angularDamping = rigidbody->m_Settings.angularDamping;
    bodyDef.gravityScale = rigidbody->m_Settings.gravityScale;
    bodyDef.allowSleep = rigidbody->m_Settings.allowSleep;
    bodyDef.awake = rigidbody->m_Settings.awake;
    bodyDef.fixedRotation = rigidbody->m_Settings.fixedRotation;
    bodyDef.bullet = rigidbody->m_Settings.avoidTunnelingOnDynamicBodies;
    bodyDef.active = rigidbody->m_Settings.active;

    rigidbody->m_BodyPtr = m_World->CreateBody(&bodyDef);
}

void jul::Physics::RemoveRidgidbody(Rigidbody* rigidbody) { m_World->DestroyBody(rigidbody->m_BodyPtr); }
