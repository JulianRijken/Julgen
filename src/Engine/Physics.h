#pragma once
#include <Box2D/Box2D.h>

#include "Rigidbody.h"
#include "Service.h"

namespace jul
{

    class Physics final : public jul::Service
    {
        friend class Rigidbody;

    public:
        Physics();

        void FixedUpdate();

    private:
        void AddRidgidbody(Rigidbody* rigidbody);
        void RemoveRidgidbody(Rigidbody* rigidbody);


        std::unique_ptr<b2World> m_World;
        int32 m_VelocityIterations = 6;
        int32 m_PositionIterations = 2;
    };

}  // namespace jul
