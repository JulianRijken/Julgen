#pragma once
#include <Box2D/Box2D.h>

#include "BoxCollider.h"
#include "Rigidbody.h"
#include "Service.h"

namespace jul
{

    class Physics final : public Service
    {
    public:
        Physics();

        void FixedUpdate();

        void AddRidgidbody(Rigidbody* rigidbodyPtr);
        void RemoveRidgidbody(Rigidbody* rigidbodyPtr);

        void AddCollider(BoxCollider* colliderPtr);
        void RemoveCollider(BoxCollider* colliderPtr);


    private:
        std::unique_ptr<b2World> m_World;
        int32 m_VelocityIterations = 6;
        int32 m_PositionIterations = 2;
    };

}  // namespace jul
