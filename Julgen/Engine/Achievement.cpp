#include "Achievement.h"
#include <iostream>

#ifdef USE_STEAMWORKS
#include <steam_api.h>
#endif

void jul::Achievement::Unlock(AchievementType unlockType)
{
#ifdef USE_STEAMWORKS

    switch (unlockType)
    {

    case AchievementType::Winner:
        SteamUserStats()->SetAchievement("ACH_WIN_ONE_GAME");
        break;
    case AchievementType::Death:
        SteamUserStats()->SetAchievement("ACH_TRAVEL_FAR_ACCUM");
        break;
    }


    bool success = SteamUserStats()->StoreStats();
    if (success)
        std::cout << "Achievement Unlocked: " << (int)unlockType << std::endl;
     else
        std::cout << "Failed to unlock achievement: " << (int)unlockType << std::endl;

#else
    std::cout << "Achievment Unlocked: " << (int)unlockType << std::endl;
    std::cout << "Steamworks not initialized" << std::endl;
#endif
}

