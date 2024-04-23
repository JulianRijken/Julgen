#pragma once
#include <Box2D/Box2D.h>

#include "Service.h"

namespace jul
{

    class Physics final : public jul::Service
    {
    public:
        Physics();

        void FixedUpdate();

        // TODO: Should be abstracted away in to functions
        b2World& GetWorld() { return *m_World; }

    private:
        std::unique_ptr<b2World> m_World;
        int32 m_VelocityIterations = 6;
        int32 m_PositionIterations = 2;
    };

}  // namespace jul
