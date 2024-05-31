#pragma once

#include <algorithm>

#include "glm/exponential.hpp"


namespace jul::math
{

    // TODO: For the random stuff consider std::rand or random number generator
    //       this also to make it thread safe
    template<typename Type = double>
        requires std::floating_point<Type>
    constexpr Type RandomValue()
    {
        return static_cast<Type>(rand() % RAND_MAX) / static_cast<Type>(RAND_MAX);
    };

    template<typename Type>
        requires std::floating_point<Type>
    constexpr Type RandomRange(Type min, Type max)
    {
        if (min > max)
        {
            const Type TEMP_MAX = max;
            max = min;
            min = TEMP_MAX;
        }

        const Type RANDOM_ALPHA{ Type(rand()) / RAND_MAX };
        const Type RANGE{ max - min };

        return RANDOM_ALPHA * RANGE + min;
    }

    template<typename Type>
        requires std::integral<Type>
    constexpr Type RandomRange(Type min, Type max)
    {
        if (min > max)
        {
            Type tempMax = max;
            max = min;
            min = tempMax;
        }

        return rand() % (max - min + 1) + min;
    }

    template<typename Type>
    constexpr const Type& Clamp01(const Type& value)
    {
        return std::clamp(value, static_cast<Type>(0), static_cast<Type>(1));
    };

    template<typename Type>
        requires std::integral<Type> or std::floating_point<Type>
    constexpr Type ClampLoop(Type value, Type min, Type max)
    {
        if(min == max)
            return value;

        if (min > max)
        {
            const Type TEMP_MAX = max;
            max = min;
            min = TEMP_MAX;
        }

        Type range = max - min;
        Type result = value;

        while (result > max)
            result -= range;

        while (result < min)
            result += range;

        return result;
    }

    template<typename Type>
        requires std::integral<Type> or std::floating_point<Type>
    constexpr Type ClampLoop01(Type value)
    {
        return ClampLoop(value, static_cast<Type>(0), static_cast<Type>(1));
    };

    template<typename Type>
        requires std::integral<Type> or std::floating_point<Type>
    Type LerpSmooth(const Type& a, const Type& b, float deltaTime, float duration)
    {
        // half life (2)
        const float h{ -duration / glm::log2(1.0f / 1000.0f) };

        return b + (a - b) * glm::exp2(-deltaTime / h);
    }

    template<typename Type, typename DeltaType>
        requires std::integral<Type> or std::floating_point<Type>
    Type MoveTowards(const Type& a, const Type& b, DeltaType maxDelta)
    {
        if(std::fabs(b - a) <= maxDelta)
            return b;

        return a + std::copysign(maxDelta, b - a);
    }

    template<typename Type>
        requires std::floating_point<Type>
    constexpr Type MapWave(const Type time, const Type min, const Type max, const Type repeatTime, const Type startTime)
    {
        const Type amplitude{ (max - min) / 2.0f };
        const Type intercept{ min + amplitude };
        const Type pulsation{ 2 * std::numbers::pi_v<Type> / repeatTime };
        const Type phase{ 2 * std::numbers::pi_v<Type> * startTime };

        return amplitude * std::sin(pulsation * time + phase) + intercept;
    }
}
