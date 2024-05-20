#pragma once

#include <functional>

namespace jul
{

    class GameObject;

    struct Tween
    {
        float from = 0.0f;
        float to = 1.0f;
        float duration = 1.0f;
        bool igunoreTimeScale = false;

        std::function<void()> onStart = {};
        std::function<void(float)> onUpdate = {};
        std::function<void()> onEnd = {};
    };

}  // namespace jul
