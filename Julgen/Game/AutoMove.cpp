#include "AutoMove.h"

#include "GameObject.h"
#include "GameTime.h"
#include "Transform.h"

AutoMove::AutoMove(glm::vec3  velocity) :
	Behaviour{"AutoMove"},
	m_Velocity{velocity}
{}

void AutoMove::Update()
{
	Transform().Translate(m_Velocity * jul::GameTime::GetDeltaTimeF());
}
