#pragma once

#include <algorithm>


namespace jul::math
{

    // TODO: For the random stuff consider std::rand or random number generator
    //       this also to make it thread safe
    template<typename Type>
        requires std::floating_point<Type>
    constexpr Type RandomValue()
    {
        return static_cast<float>(rand() % RAND_MAX) / static_cast<float>(RAND_MAX);
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
        requires std::integral<Type> || std::floating_point<Type>
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
        requires std::integral<Type> || std::floating_point<Type>
    constexpr Type ClampLoop01(Type value)
    {
        return ClampLoop(value, static_cast<Type>(0), static_cast<Type>(1));
    };
}
