#pragma once


#include <functional>

#include "EaseFunction.h"

namespace jul
{

    class GameObject;

    struct Tween
    {
        double from = 0.0f;
        double to = 1.0f;
        double duration = 1.0f;
        bool igunoreTimeScale = false;
        EaseFunction::Type easeFunction = EaseFunction::Type::Linear;

        std::function<void()> onStart = {};
        std::function<void(double)> onUpdate = {};
        std::function<void()> onEnd = {};
    };

}  // namespace jul
