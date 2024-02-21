#include "Transform.h"

//jul::Transform::Transform(const glm::vec3& position) :
//	m_Position(position)
//{
//
//}


jul::Transform::Transform(glm::vec3 position) :
	m_Position{position}
{
	
}

void jul::Transform::SetPosition(const float x, const float y, const float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}


