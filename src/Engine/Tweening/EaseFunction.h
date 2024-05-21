#pragma once

#include <cmath>
#include <numbers>
#include <stdexcept>

namespace jul
{
    class EaseFunction
    {
    public:
        enum class Type
        {
            Linear,
            SineIn,
            SineOut,
            SineInOut,
            BounceIn,
            BounceOut,
            BounceInOut
        };

        using enum Type;

        static constexpr double LinearLerp(double time) { return time; }

        static constexpr double SineInLerp(double time) { return 1.0 - std::cos((time * std::numbers::pi) / 2.0); }

        static constexpr double SineOutLerp(double time) { return std::sin((time * std::numbers::pi) / 2.0f); }

        static constexpr double SineInOutLerp(double time)
        {
            return -(std::cos(std::numbers::pi * time) - 1.0f) / 2.0f;
        }

        // TODO: Get rid of the magic numbers lol
        static constexpr float CONSTANT_F = 7.5625;
        static constexpr float CONSTANT_G = 2.75;

        static constexpr double BounceInLerp(double time) { return 1 - BounceOutLerp(1 - time); }

        static constexpr double BounceOutLerp(double time)
        {

            if(time < 1 / CONSTANT_G)
                return CONSTANT_F * time * time;

            if(time < 2 / CONSTANT_G)
            {
                time -= 1.5 / CONSTANT_G;
                return CONSTANT_F * time * time + 0.75;
            }

            if(time < 2.5 / CONSTANT_G)
            {
                time -= 2.25 / CONSTANT_G;
                return CONSTANT_F * time * time + 0.9375;
            }


            time -= 2.625 / CONSTANT_G;
            return CONSTANT_F * time * time + 0.984375;
        }

        static constexpr double BounceInOutLerp(double time)
        {
            return time < 0.5 ? (1 - BounceOutLerp(1 - 2 * time)) / 2 : (1 + BounceOutLerp(2 * time - 1)) / 2;
        }

        static constexpr double Evaluate(double time, Type type)
        {
            switch(type)
            {
                case Type::Linear:
                    return LinearLerp(time);
                    break;
                case Type::SineIn:
                    return SineInLerp(time);
                    break;
                case Type::SineOut:
                    return SineOutLerp(time);
                    break;
                case Type::SineInOut:
                    return SineInLerp(time);
                    break;
                case Type::BounceIn:
                    return BounceInLerp(time);
                    break;
                case Type::BounceOut:
                    return BounceOutLerp(time);
                    break;
                case Type::BounceInOut:
                    return BounceInOutLerp(time);
                    break;
            }

            throw std::runtime_error("Ease Function Not Defined");
            return 0.0f;
        }
    };
}  // namespace jul
