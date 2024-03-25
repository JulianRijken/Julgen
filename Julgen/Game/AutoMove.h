#pragma once
#include "Component.h"
#include <glm/vec3.hpp>

class AutoMove final : public jul::Component
{
public:

    AutoMove(jul::GameObject* parent,const glm::vec3& velocity = {},const glm::vec3& boundingBox = {},const glm::vec3& boundingBoxCenter = {});

private:

	void Update() override;

	glm::vec3 m_Velocity{};
    glm::vec3 m_BoundingBox{};
    glm::vec3 m_BoundingBoxCenter{};
};

