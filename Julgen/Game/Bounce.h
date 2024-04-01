#pragma once

#include "Component.h"

#include <glm/vec3.hpp>

class Bounce final : public jul::Component
{
public:

	Bounce(jul::GameObject* parent, float offset = 0.0f, float restitution = 0.8f);

private:

	void FixedUpdate() override;

	glm::vec3 m_Velocity{};

	float m_Offset{};
	float m_Restitution{ 0.0f};
    const float GRAVITY{ 400.0 };
};

