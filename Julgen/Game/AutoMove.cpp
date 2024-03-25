#include "AutoMove.h"

#include "GameTime.h"
#include "Transform.h"

AutoMove::AutoMove(jul::GameObject* parent,
                   const glm::vec3& velocity,
                   const glm::vec3& boundingBox,
                   const glm::vec3& boundingBoxCenter)
    : Component(parent, "AutoMove"), m_Velocity(velocity), m_BoundingBox(boundingBox),
      m_BoundingBoxCenter(boundingBoxCenter)
{}

void AutoMove::Update()
{
	Transform().Translate(m_Velocity * jul::GameTime::GetDeltaTimeF());

    glm::vec3 currentPosition{Transform().WorldPosition()};

    if(m_BoundingBox.x > 0 or m_BoundingBox.y > 0 or m_BoundingBox.z > 0)
    {
        if (currentPosition.x > m_BoundingBox.x + m_BoundingBoxCenter.x)
            currentPosition.x -= 2 * (m_BoundingBox.x + m_BoundingBoxCenter.x);
        else if (currentPosition.x < -m_BoundingBox.x + m_BoundingBoxCenter.x)
            currentPosition.x += 2 * (m_BoundingBox.x + m_BoundingBoxCenter.x);

        if (currentPosition.y > m_BoundingBox.y + m_BoundingBoxCenter.y)
            currentPosition.y -= 2 * (m_BoundingBox.y + m_BoundingBoxCenter.y);
        else if (currentPosition.y < -m_BoundingBox.y + m_BoundingBoxCenter.y)
            currentPosition.y += 2 * (m_BoundingBox.y + m_BoundingBoxCenter.y);

        if (currentPosition.z > m_BoundingBox.z + m_BoundingBoxCenter.z)
            currentPosition.z -= 2 * (m_BoundingBox.z + m_BoundingBoxCenter.z);
        else if (currentPosition.z < -m_BoundingBox.z + m_BoundingBoxCenter.z)
            currentPosition.z += 2 * (m_BoundingBox.z + m_BoundingBoxCenter.z);
    }

    Transform().SetWorldPosition(currentPosition);
}
