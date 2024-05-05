#pragma once

#include "Box2D/Dynamics/b2Body.h"

namespace jul
{
    class Collision;

    class ICollisionListener
    {
    public:
        virtual ~ICollisionListener() = default;

        ICollisionListener(const ICollisionListener&) = delete;
        ICollisionListener(ICollisionListener&&) noexcept = delete;
        ICollisionListener& operator=(const ICollisionListener&) = delete;
        ICollisionListener& operator=(ICollisionListener&&) noexcept = delete;

        virtual void OnCollisionBegin(b2Contact* /*unused*/, b2Fixture* /*unused*/){};
        virtual void OnCollisionEnd(b2Contact* /*unused*/, b2Fixture* /*unused*/){};
        virtual void OnCollisionPreSolve(b2Contact* /*unused*/, b2Fixture* /*unused*/){};
        virtual void OnCollisionPostSolve(b2Contact* /*unused*/, b2Fixture* /*unused*/){};

    protected:
        ICollisionListener() = default;
    };

}  // namespace jul
