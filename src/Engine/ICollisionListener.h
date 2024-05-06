#pragma once

#include "Physics.h"


namespace jul
{
    struct Collision;

    class ICollisionListener
    {
    public:
        virtual ~ICollisionListener() = default;

        ICollisionListener(const ICollisionListener&) = delete;
        ICollisionListener(ICollisionListener&&) noexcept = delete;
        ICollisionListener& operator=(const ICollisionListener&) = delete;
        ICollisionListener& operator=(ICollisionListener&&) noexcept = delete;

        virtual void OnCollisionBegin(Collision /*unused*/){};
        virtual void OnCollisionEnd(Collision /*unused*/){};
        virtual void OnCollisionPreSolve(Collision /*unused*/, const b2Manifold*){};
        virtual void OnCollisionPostSolve(Collision /*unused*/){};

    protected:
        ICollisionListener() = default;
    };

}  // namespace jul
