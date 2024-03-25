#pragma once

#include "Component.h"

#include <glm/vec3.hpp>

class AutoRotateAround : public jul::Component
{
public:

    AutoRotateAround(jul::GameObject* parent,float distanceFromTarget, float speed = 1.0f, const glm::vec3& rotateAroundPoint = glm::vec3{0,0,0});

private:
	void Update() override;

	float m_Speed{};
	float m_DistanceFromTarget{};
	glm::vec3 m_RotateAroundPoint{};
};
