#pragma once
#include <Box2D/Box2D.h>

#include "Component.h"

class Rigidbody final : public jul::Component
{
public:
    Rigidbody(jul::GameObject* parent);

    Rigidbody(const Rigidbody&) = delete;
    Rigidbody(Rigidbody&&) noexcept = delete;
    Rigidbody& operator=(const Rigidbody&) = delete;
    Rigidbody& operator=(Rigidbody&&) noexcept = delete;

    // TODO: Decide between abstracting box 2D or returning this
    b2Body& GetBody() { return *m_BodyPtr; }


    void FixedUpdate() override;
private:
    b2Body* m_BodyPtr;  // Owned by world
};

