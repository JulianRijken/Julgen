#include "Bounce.h"

#include "GameTime.h"
#include "Transform.h"

using namespace glm;

Bounce::Bounce(jul::GameObject* parent, float offset, float restitution) :
	Component(parent,"Bounce"),
	m_Offset(offset),
	m_Restitution(restitution)
{}


void Bounce::FixedUpdate()
{
	vec3 pos = Transform().WorldPosition();
	pos += m_Velocity * jul::GameTime::GetFixedDeltaTimeF();


	const float ground = jul::GlobalSettings::WINDOW_HEIGHT - m_Offset;


	if (pos.y > ground)
	{
		// Add overshot
		pos.y -= pos.y - ground;

		m_Velocity.y = -m_Velocity.y * m_Restitution;
	}

	m_Velocity.y += jul::GameTime::GetFixedDeltaTimeF() * GRAVITY;

	Transform().SetLocalPosition(pos);
}
