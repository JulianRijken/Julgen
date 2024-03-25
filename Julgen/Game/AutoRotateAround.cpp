#include "AutoRotateAround.h"

#include "GameTime.h"
#include "Transform.h"


AutoRotateAround::AutoRotateAround(jul::GameObject* parent, float distanceFromTarget, float speed,const glm::vec3& rotateAroundPoint) :
    Component(parent,"AutoRotateAround"),
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
