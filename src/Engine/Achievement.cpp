#include "Achievement.h"
// #include "MessageQueue.h"

#include <cassert>
#include <iostream>

#ifdef USE_STEAMWORKS
    #ifdef _MSC_VER
    #pragma warning (push)
    #pragma warning (disable: 4996)
    #endif

    #include <steam_api.h>

    #ifdef _MSC_VER
    #pragma warning (pop)
    #endif
#endif

void jul::Achievement::Initialize()
{
    // TODO: Achievements should be part of the game and not the engine unless abstracted further
    // MessageQueue::AddListener(MessageType::PlayerScoreChanged,this,&Achievement::OnScoreChange);
}

void jul::Achievement::OnScoreChange(const Message& message)
{
    try
    {
        const int score = std::any_cast<int>(message.arguments[0]);

        if(score >= 500)
            Unlock(AchievementType::Winner);
    }
    catch(...)
    {
        std::cerr << "Message send with wrong arguments" << '\n';
        assert(false);
    }
}

void jul::Achievement::Unlock(AchievementType unlockType)
{
#ifdef USE_STEAMWORKS

    switch (unlockType)
    {
    case AchievementType::Winner:
        SteamUserStats()->SetAchievement("ACH_WIN_ONE_GAME");
        break;
    }

    if (SteamUserStats()->StoreStats())
        std::cout << "Achievement Unlocked: " << static_cast<int>(unlockType) << '\n';
    else
        std::cout << "Failed to unlock achievement: " << static_cast<int>(unlockType) << '\n';

#else
    std::cout << "Achievment Unlocked: " << (int)unlockType << '\n';
    std::cout << "Steamworks not initialized" << '\n';
#endif
}

