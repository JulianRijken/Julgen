#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <Box2D/Dynamics/b2WorldCallbacks.h>

#include <glm/vec2.hpp>
#include <unordered_set>

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
            Force,
            VelocityChange
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
            bool manuallyMove = false;

            static Settings Default() { return {}; }
        };

        Rigidbody(GameObject* parentPtr, const Settings& settings = Settings::Default());
        ~Rigidbody() override;

        Rigidbody(const Rigidbody&) = delete;
        Rigidbody(Rigidbody&&) noexcept = delete;
        Rigidbody& operator=(const Rigidbody&) = delete;
        Rigidbody& operator=(Rigidbody&&) noexcept = delete;

        glm::vec2 Position() const;
        glm::vec2 GetVelocity() const;
        glm::vec2 LinearVelocityFromWorldPoint(glm::vec2 worldPoint) const;

        void SetGravityScale(float scale);

        void AddForce(glm::vec2 force, ForceMode forceMode = ForceMode::Force, bool wake = true);
        void SetPosition(glm::vec2 position) const;
        void FixedUpdate() override;
        void SetMode(Mode mode);
        void SetVelocity(glm::vec2 velocity) const;

        [[nodiscard]] b2Body* GetBody() const { return m_BodyPtr; }

        [[nodiscard]] const Settings& GetSettings() const { return m_Settings; }

        void UpdateCollisionListeners();

    private:
        void OnCollisionBegin(const Collision& collision) const;
        void OnCollisionEnd(const Collision& collision) const;
        void OnCollisionPreSolve(const Collision& collision, const b2Manifold* oldManifold) const;
        void OnCollisionPostSolve(const Collision& collision) const;

        b2Body* m_BodyPtr{};  // Owned by world
        Settings m_Settings{};

        Mode m_ModeFlag{};
        std::unordered_set<ICollisionListener*> m_CollisionListeners;
    };

}  // namespace jul
#endif // RIGIDBODY_H
