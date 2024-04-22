#pragma once
#include <Box2D/Box2D.h>

class Physics final
{

public:
    Physics();

    void FixedUpdate();

    b2World& GetWorld() { return *m_World; }

private:
    std::unique_ptr<b2World> m_World;
    int32 m_VelocityIterations = 6;
    int32 m_PositionIterations = 2;
};
