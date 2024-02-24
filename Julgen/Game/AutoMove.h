#pragma once
#include "Behaviour.h"
#include "glm/vec3.hpp"

class AutoMove final : public jul::Behaviour
{
public:

	AutoMove(glm::vec3 velocity = {});

private:

	void Update() override;

	glm::vec3 m_Velocity{};

};

