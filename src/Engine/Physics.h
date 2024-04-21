#pragma once
#include <Box2D/Box2D.h>

#include "Singleton.h"

class Physics : public jul::Singleton<Physics>
{
public:
    void Initialize();
    void FixedUpdate();

    b2World& GetWorld() { return *m_World; }

private:
    std::unique_ptr<b2World> m_World;
    int32 m_VelocityIterations = 6;
    int32 m_PositionIterations = 2;
};
