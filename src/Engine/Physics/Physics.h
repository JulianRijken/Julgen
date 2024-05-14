#pragma once
#include <Box2D/Box2D.h>

#include "BoxCollider.h"
#include "Rigidbody.h"
#include "Service.h"
#include <functional>

namespace jul
{
    struct Collision
    {
        b2Fixture* thisFixture;
        b2Fixture* otherFixture;
        b2Contact* contact;
    };

    struct RayCastResult
    {
        glm::vec2 point{};
        glm::vec2 normal{};
        float distance{ 0.0f };
        bool hit{ false };
        BoxCollider* hitCollider{};
    };

    class Physics final : public Service, public b2ContactListener
    {
    public:
        Physics();

        void FixedUpdate();

        void AddRigidbody(Rigidbody* rigidbodyPtr);
        void RemoveRigidbody(const Rigidbody* rigidbodyPtr);

        void AddCollider(BoxCollider* colliderPtr);
        void RemoveCollider(const BoxCollider* colliderPtr);

        static bool RayCast(glm::vec2 from, glm::vec2 direction, float distance);
        static bool RayCast(glm::vec2 from, glm::vec2 direction, float distance, jul::RayCastResult& result);

    private:
        class RayCastCallback final : public b2RayCastCallback
        {
        public:
            RayCastResult result;

            float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal,
                                  float32 fraction) override
            {
                result.point = glm::vec2(point.x, point.y);
                result.normal = glm::vec2(normal.x, normal.y);
                result.distance = fraction;
                result.hit = true;
                result.hitCollider = static_cast<BoxCollider*>(fixture->GetUserData());
                return fraction;
            }
        };

        std::unique_ptr<b2World> m_World;
        int32 m_VelocityIterations = 6;
        int32 m_PositionIterations = 2;

        void BeginContact(b2Contact* contact) override;
        void EndContact(b2Contact* contact) override;
        void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;
        void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;

        void HandleContact(b2Contact* contact, const std::function<void(Rigidbody*, Collision)>& callback) const;
    };

}  // namespace jul
