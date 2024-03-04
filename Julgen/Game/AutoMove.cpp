#include "AutoMove.h"

#include "GameTime.h"
#include "Transform.h"

AutoMove::AutoMove(jul::GameObject* parent, glm::vec3  velocity) :
	Component(parent,"AutoMove"),
	m_Velocity(velocity)
{}

void AutoMove::Update()
{
	Transform().Translate(m_Velocity * jul::GameTime::GetDeltaTimeF());
}
