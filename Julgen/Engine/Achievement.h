#pragma once

#include "Singleton.h"

namespace  jul
{

    enum class AchievementType
    {
        Winner,
        Death
    };


    class Achievement final : public Singleton<Achievement>
    {
    public:
        void Unlock(AchievementType unlockType);
    };
}
