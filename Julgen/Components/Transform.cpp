#include "Transform.h"


jul::Transform::Transform(glm::vec3 position) :
	m_Position{position}
{}

void jul::Transform::SetPosition(const float x, const float y, const float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}

void jul::Transform::SetPosition(const glm::vec3& position)
{
	m_Position = position;
}

void jul::Transform::Translate(const glm::vec3& translation)
{
	m_Position += translation;
}



