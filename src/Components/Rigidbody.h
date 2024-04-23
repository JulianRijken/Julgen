#pragma once

#include <glm/vec2.hpp>

#include "Component.h"

class b2Body;

namespace jul
{
    enum class ForceMode
    {
        Impulse,
        Force
    };


    class Rigidbody final : public jul::Component
    {
    public:
        Rigidbody(jul::GameObject* parent);

        Rigidbody(const Rigidbody&) = delete;
        Rigidbody(Rigidbody&&) noexcept = delete;
        Rigidbody& operator=(const Rigidbody&) = delete;
        Rigidbody& operator=(Rigidbody&&) noexcept = delete;

        glm::vec2 Positon();
        glm::vec2 Velicty();
        void AddForce(glm::vec2 force, ForceMode forceMode, bool wake = true);
        void SetPosition(glm::vec2 position);
        void FixedUpdate() override;

    private:
        b2Body* m_BodyPtr;  // Owned by world
    };

}  // namespace jul
