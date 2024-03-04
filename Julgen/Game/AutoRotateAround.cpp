#include "AutoRotateAround.h"
#include <complex>

#include "GameTime.h"

#include "SceneManager.h"
#include "Transform.h"


AutoRotateAround::AutoRotateAround(float distanceFromTarget, float speed, glm::vec3 rotateAroundPoint) :
	Component("AutoRotateAround"),
	m_Speed(speed),
	m_DistanceFromTarget(distanceFromTarget),
	m_RotateAroundPoint(rotateAroundPoint)
{}

void AutoRotateAround::Update()
{
	glm::vec3 targetPosition = m_RotateAroundPoint;
	targetPosition += glm::vec3
	{
		std::cos(jul::GameTime::GetElapsedTime() * m_Speed) * m_DistanceFromTarget,
		std::sin(jul::GameTime::GetElapsedTime() * m_Speed) * m_DistanceFromTarget,
		0
	};


	Transform().SetLocalPosition(targetPosition);
} 
