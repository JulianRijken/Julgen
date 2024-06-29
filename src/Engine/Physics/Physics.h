#ifndef PHYSICS_H
#define PHYSICS_H
#include <Box2D/Box2D.h>
#include <fmt/core.h>

#include <functional>

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

    struct RayCastResult
    {
        glm::vec2 point{};
        glm::vec2 normal{};
        float distance{ 0.0f };
        bool hit{ false };
        BoxCollider* hitCollider{ nullptr };
    };

    class Physics final : public Service, public b2ContactListener
    {
    public:
        Physics();

        void UpdateIsActive();
        void UpdateWorld();

        void AddRigidbody(Rigidbody* rigidbodyPtr);
        void RemoveRigidbody(const Rigidbody* rigidbodyPtr);

        void AddCollider(BoxCollider* colliderPtr);
        void RemoveCollider(const BoxCollider* colliderPtr);

        static bool RayCast(glm::vec2 from, glm::vec2 direction, float distance, uint16_t collisionMask = 0xFFFF);
        static bool RayCast(glm::vec2 from, glm::vec2 direction, float distance, jul::RayCastResult& result,
                            uint16_t collisionMask = 0xFFFF);

    private:
        // Used as a wrapper around the cast callback
        // Alows a raycast to return a result instead of needing to implement the callback
        class RayCastCallback final : public b2RayCastCallback
        {
        public:
            RayCastCallback(uint16_t checkBits) :
                m_CheckBits(checkBits)
            {
            }

            [[nodiscard]] RayCastResult GetResult() const { return m_Result; }


        private:
            // Will be called multple times
            float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal,
                                  float32 fraction) override
            {
                if(not(fixture->GetFilterData().categoryBits & m_CheckBits))
                    return fraction;

                m_Result.point = glm::vec2(point.x, point.y);
                m_Result.normal = glm::vec2(normal.x, normal.y);
                m_Result.distance = fraction;
                m_Result.hit = true;
                m_Result.hitCollider = static_cast<BoxCollider*>(fixture->GetUserData());
                return fraction;
            }

            RayCastResult m_Result{};
            uint16_t m_CheckBits{};
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
#endif // PHYSICS_H
