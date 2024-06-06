#pragma once

#include <Box2D/Box2D.h>

#include "Component.h"
#include "Rigidbody.h"

namespace jul
{
    class BoxCollider final : public Component
    {
        friend class Physics;

    public:
        struct Mask
        {
            /// The collision category bits. Normally you would just set one bit.
            uint16_t category = 0x0001;

            /// The collision mask bits. This states the categories that this
            /// shape would accept for collision.
            /// DEFAULT TO ALL
            uint16_t collideWith = 0xFFFF;

            /// Collision groups allow a certain group of objects to never collide (negative)
            /// or always collide (positive). Zero means no collision group. Non-zero group
            /// filtering always wins against the mask bits.
            uint16_t groupIndex = 0;
        };

        struct Settings
        {
            float friction{ 0.2f };
            float restitution{ 0.0f };
            float density{ 0.0f };

            glm::vec2 size{ 1.0f, 1.0f };
            glm::vec2 center{ 0, 0 };

            Mask mask{};

            bool isSensor{ false };

            static Settings Default() { return {}; }
        };

        BoxCollider(GameObject* parentPtr, const Settings& setting = Settings::Default(),
                    Rigidbody* connectedBody = nullptr);
        ~BoxCollider() override;

        void SetRestitution(float restitution);
        void SetSensor(bool sensor);

        [[nodiscard]] const b2Body& GetBody() const { return *m_BodyPtr; }

        [[nodiscard]] const Settings& GetSettings() const { return m_Settings; }

    private:
        b2Body* m_BodyPtr{ nullptr };
        b2Fixture* m_Fixture{ nullptr };
        Rigidbody* m_ConnectedRigidbody{ nullptr };
        Settings m_Settings{};
        bool m_ActiveFlag{ true };
    };
}  // namespace jul
