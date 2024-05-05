#pragma once
#include <Box2D/Box2D.h>

#include "BoxCollider.h"
#include "Rigidbody.h"
#include "Service.h"

namespace jul
{
    struct Collision
    {
        b2Fixture* thisFixture;
        b2Fixture* otherFixture;

        b2Contact* contact;
    };

    class Physics final : public Service, public b2ContactListener
    {
    public:
        Physics();

        void FixedUpdate();

        void AddRidgidbody(Rigidbody* rigidbodyPtr);
        void RemoveRidgidbody(Rigidbody* rigidbodyPtr);

        void AddCollider(BoxCollider* colliderPtr);
        void RemoveCollider(BoxCollider* colliderPtr);


    private:
        std::unique_ptr<b2World> m_World;
        int32 m_VelocityIterations = 6;
        int32 m_PositionIterations = 2;

        void BeginContact(b2Contact* contact) override;
        void EndContact(b2Contact* contact) override;
        void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;
        void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;

        void HandleContact(b2Contact* contact, std::function<void(Rigidbody*, Collision)> callback);
    };

}  // namespace jul
