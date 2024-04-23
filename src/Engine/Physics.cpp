#include "Physics.h"

#include <memory>

#include "GameTime.h"

jul::Physics::Physics() :
    m_World(std::make_unique<b2World>(b2Vec2{ 0, -9.81f }))
{
}

void jul::Physics::FixedUpdate()
{
    m_World->Step(jul::GameTime::GetFixedDeltaTimeF(), m_VelocityIterations, m_PositionIterations);
}
