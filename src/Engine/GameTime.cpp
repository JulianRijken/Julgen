#include "GameTime.h"

#include <chrono>


void jul::GameTime::Update()
{
    static std::chrono::time_point<std::chrono::high_resolution_clock> s_LastTime{
        std::chrono::high_resolution_clock::now()
    };

    // Calculate delta time
    const auto currentTime = std::chrono::high_resolution_clock::now();
    g_DeltaTime = std::min(MAX_DELTA_TIME, std::chrono::duration<double>(currentTime - s_LastTime).count());
    s_LastTime = currentTime;

    // Update elapsed time (Scaled)
    g_ElapsedTime += GetDeltaTime();


    // Update Avrage FPS
    g_FpsUpdateTimer += g_DeltaTime;
    g_AccumulatedDeltaTime += g_DeltaTime;
    g_FramesCountedSinceLastAverage++;
    if(g_FpsUpdateTimer > SMOOTH_FPS_UPDATE_RATE)
    {
        g_FpsUpdateTimer -= SMOOTH_FPS_UPDATE_RATE;
        g_AverageFps = 1.0 / (g_AccumulatedDeltaTime / g_FramesCountedSinceLastAverage);
        g_AccumulatedDeltaTime = 0.0;
        g_FramesCountedSinceLastAverage = 0;
    }
}
