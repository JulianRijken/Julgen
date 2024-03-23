#pragma once

#include <algorithm>


namespace jul::math
{
    template<typename Type>
    requires std::floating_point<Type>
    constexpr const Type RandomValue()
    {
        return rand() % RAND_MAX / float(RAND_MAX);
    };

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
            const Type tempMax = max;
            max = min;
            min = tempMax;
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
