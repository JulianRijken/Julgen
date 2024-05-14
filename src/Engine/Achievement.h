#pragma once

#include "MessageQueue.h"
#include "Singleton.h"

namespace  jul
{
    // TODO: Enum should be defined outside the engine
    enum class AchievementType
    {
        Winner,
    };

    class Achievement final : public Singleton<Achievement>
    {
    public:
        void Initialize();

    private:
        void OnScoreChange(const Message& message);
        void Unlock(AchievementType unlockType);
    };
}
