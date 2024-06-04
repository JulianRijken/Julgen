#pragma once


#include <functional>

#include "EaseFunction.h"

namespace jul
{

    class GameObject;

    struct Tween
    {
        double delay = 0.0;
        double from = 0.0;
        double to = 1.0;
        double duration = 1.0;
        bool igunoreTimeScale = false;
        bool invokeWhenDestroyed = true;
        EaseFunction::Type easeFunction = EaseFunction::Type::Linear;

        std::function<void()> onStart = {};
        std::function<void(double)> onUpdate = {};
        std::function<void()> onEnd = {};
    };

}  // namespace jul
