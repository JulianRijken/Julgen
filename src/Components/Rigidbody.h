#pragma once

#include <Box2D/Dynamics/b2WorldCallbacks.h>

#include <glm/vec2.hpp>
#include <vector>

#include "Component.h"

class b2Body;

namespace jul
{

    class ICollisionListener;

    struct Collision;
    class BoxCollider;

    class Rigidbody final : public Component
    {
        friend class Physics;
        friend class BoxCollider;

    public:
        enum class ForceMode
        {
            Impulse,
            Force
        };

        /// Static: zero mass, zero velocity, may be manually moved.
        /// Kinematic: zero mass, non-zero velocity set by user, moved by solver.
        /// Dynamic: positive mass, non-zero velocity determined by forces, moved by solver.
        enum class Mode
        {
            Static,
            Kinematic,
            Dynamic
        };

        struct Settings
        {
            Mode mode = Mode::Dynamic;
            float angle = 0.0f;
            float linearDamping = 0.0f;
            float angularDamping = 0.0f;
            float gravityScale = 1.0f;
            bool allowSleep = true;
            bool awake = true;
            bool fixedRotation = false;
            bool avoidTunnelingOnDynamicBodies = false;
            bool active = true;

            static Settings Default() { return {}; }
        };

        Rigidbody(GameObject* parentPtr, Settings setting = Settings::Default());
        ~Rigidbody() override;

        Rigidbody(const Rigidbody&) = delete;
        Rigidbody(Rigidbody&&) noexcept = delete;
        Rigidbody& operator=(const Rigidbody&) = delete;
        Rigidbody& operator=(Rigidbody&&) noexcept = delete;

        glm::vec2 Positon();
        glm::vec2 Velocity();
        void AddForce(glm::vec2 force, ForceMode forceMode, bool wake = true);
        void SetPosition(glm::vec2 position);
        void FixedUpdate() override;

        [[nodiscard]] const Settings& GetSettings() const { return m_Settings; }


    private:
        void OnCollisionBegin(b2Contact* collision);
        void OnCollisionEnd(b2Contact* collision);
        void OnCollisionPreSolve(b2Contact* collision);
        void OnCollisionPostSolve(b2Contact* collision);

        b2Body* m_BodyPtr{};  // Owned by world
        Settings m_Settings{};

        std::vector<ICollisionListener*> m_CollisionListeners;
    };

}  // namespace jul
