#include "Physics.h"

#include <fmt/core.h>
#include <fmt/format.h>

#include <memory>

#include "GameObject.h"
#include "GameTime.h"
#include "Locator.h"
#include "Rigidbody.h"
#include "Transform.h"

jul::Physics::Physics() :
    m_World(std::make_unique<b2World>(b2Vec2{ 0, -9.81f }))
{
    m_World->SetContactListener(this);
}

void jul::Physics::UpdateIsActive()
{
    // Disable all rigidbodys before world step
    for(b2Body* body = m_World->GetBodyList(); body != nullptr; body = body->GetNext())
    {
        auto* rigidbody = static_cast<Rigidbody*>(body->GetUserData());
        if(rigidbody)
            body->SetActive(rigidbody->GetGameObject()->IsActiveInHierarchy());
    }
}

void jul::Physics::UpdateWorld()
{
    m_World->Step(jul::GameTime::GetFixedDeltaTime<float>(), m_VelocityIterations, m_PositionIterations);
}


void jul::Physics::AddRigidbody(Rigidbody* rigidbodyPtr)
{
    b2BodyDef bodyDef;  // Define body for Rigidbody based on settings
    {
        const Rigidbody::Settings& settings = rigidbodyPtr->GetSettings();

        // Make sure positions is set correct on init
        const glm::vec3 transformPosition = rigidbodyPtr->GetTransform().GetWorldPosition();
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

        bodyDef.userData = rigidbodyPtr;
    }

    rigidbodyPtr->m_BodyPtr = m_World->CreateBody(&bodyDef);
}

void jul::Physics::RemoveRigidbody(const Rigidbody* rigidbodyPtr) { m_World->DestroyBody(rigidbodyPtr->m_BodyPtr); }

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
    fixtureDef.userData = colliderPtr;

    // If collider gets connected we can just create and return
    if(colliderPtr->m_ConnectedRigidbody)
    {
        colliderPtr->m_Fixture = colliderPtr->m_ConnectedRigidbody->m_BodyPtr->CreateFixture(&fixtureDef);
        return;
    }

    b2BodyDef bodyDef;
    {
        const glm::vec3 transformPosition = colliderPtr->GetTransform().GetWorldPosition();
        bodyDef.position.Set(transformPosition.x, transformPosition.y);

        bodyDef.userData = nullptr;
        // TODO: Just the default body gets created for any non connected collider
    }

    // Create body
    colliderPtr->m_BodyPtr = m_World->CreateBody(&bodyDef);

    // Apply fixture
    colliderPtr->m_Fixture = colliderPtr->m_BodyPtr->CreateFixture(&fixtureDef);
    return;
}

void jul::Physics::RemoveCollider(const BoxCollider* colliderPtr)
{
    // body can be null when connected
    if(colliderPtr->m_BodyPtr)
        m_World->DestroyBody(colliderPtr->m_BodyPtr);
}

bool jul::Physics::RayCast(glm::vec2 from, glm::vec2 direction, float distance)
{
    RayCastResult temp{};
    return RayCast(from, direction, distance, temp);
}

bool jul::Physics::RayCast(glm::vec2 from, glm::vec2 direction, float distance, RayCastResult& result)
{
    const glm::vec2 normalizedDirection = glm::normalize(direction);
    const b2Vec2 b2From(from.x, from.y);
    const b2Vec2 b2To(from.x + normalizedDirection.x * distance, from.y + normalizedDirection.y * distance);

    RayCastCallback callback{};
    Locator::Get<Physics>().m_World->RayCast(&callback, b2From, b2To);

    result = callback.result;
    return result.hit;
}

void jul::Physics::BeginContact(b2Contact* contact)
{
    HandleContact(contact, [](const Rigidbody* rigidbody, const Collision& collision) { rigidbody->OnCollisionBegin(collision); });
}

void jul::Physics::EndContact(b2Contact* contact)
{
    HandleContact(contact, [](const Rigidbody* rigidbody, const Collision& collision) { rigidbody->OnCollisionEnd(collision); });
}

void jul::Physics::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
    HandleContact(contact,
                  [&oldManifold](const Rigidbody* rigidbody, const Collision& collision)
                  { rigidbody->OnCollisionPreSolve(collision, oldManifold); });
}

void jul::Physics::PostSolve(b2Contact* contact, const b2ContactImpulse* /*impulse*/)
{
    HandleContact(contact,
                  [](const Rigidbody* rigidbody, const Collision& collision) { rigidbody->OnCollisionPostSolve(collision); });
}

void jul::Physics::HandleContact(b2Contact* contact, const std::function<void(Rigidbody*, Collision)>& callback) const
{
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();

    for(b2Body* body = m_World->GetBodyList(); body != nullptr; body = body->GetNext())
    {
        for(b2Fixture* fixture = body->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext())
        {
            auto* rigidbody = static_cast<Rigidbody*>(body->GetUserData());
            if(rigidbody == nullptr)
                continue;

            if(fixture == fixtureA)
                callback(rigidbody, { fixtureA, fixtureB, contact });
            else if(fixture == fixtureB)
                callback(rigidbody, { fixtureB, fixtureA, contact });
        }
    }
}
