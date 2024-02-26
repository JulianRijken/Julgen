#include "Transform.h"


jul::Transform::Transform(glm::vec3 position) :
	m_Position{position}
{}

void jul::Transform::SetPosition(float x, float y, float z)
{
	SetPosition({ x,y,z });
}

void jul::Transform::SetPosition(const glm::vec3& position)
{
	m_Position = position;
}

void jul::Transform::Translate(float x, float y, float z)
{
	Translate({ x,y,z });
}

void jul::Transform::Translate(const glm::vec3& translation)
{
	m_Position += translation;
}



