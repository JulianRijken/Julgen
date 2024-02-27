#pragma once
#include "Component.h"

#include "glm/vec3.hpp"

class AutoRotateAround : public jul::Component
{
public:
	AutoRotateAround(float distanceFromTarget, float speed = 1.0f, glm::vec3 rotateAroundPoint = {});


private:
	void Update() override;


	float m_Speed{};
	float m_DistanceFromTarget{};
	glm::vec3 m_RotateAroundPoint{};
};
