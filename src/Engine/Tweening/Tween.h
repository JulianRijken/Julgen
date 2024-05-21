#pragma once

#include <cmath>
#include <functional>
#include <numbers>

namespace jul
{

    class GameObject;

    namespace easeFunction
    {
        enum class EaseType
        {
            Linear,
            SineIn,
            SineOut,
            SineInOut
        };

        static double Linear(double time) { return time; }

        static double SineIn(double time) { return 1.0 - std::cos((time * std::numbers::pi) / 2.0); }

        static double SineOut(double time) { return std::sin((time * std::numbers::pi) / 2.0f); }

        static double SineInOut(double time) { return -(std::cos(std::numbers::pi * time) - 1.0f) / 2.0f; }

        static double Ease(double time, EaseType type)
        {
            switch(type)
            {
                case jul::easeFunction::EaseType::Linear:
                    return Linear(time);
                    break;
                case jul::easeFunction::EaseType::SineIn:
                    return SineIn(time);
                    break;
                case jul::easeFunction::EaseType::SineInOut:
                    return SineInOut(time);
                    break;
                case jul::easeFunction::EaseType::SineOut:
                    return SineOut(time);
                    break;
            }
        }
    }  // namespace easeFunction

    struct Tween
    {
        double from = 0.0f;
        double to = 1.0f;
        double duration = 1.0f;
        bool igunoreTimeScale = false;

        std::function<void()> onStart = {};
        std::function<void(float)> onUpdate = {};
        std::function<void()> onEnd = {};
    };

}  // namespace jul
