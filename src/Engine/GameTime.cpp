#include "GameTime.h"

#include <chrono>
#include <numeric>

void jul::GameTime::Update()
{
    static std::chrono::time_point<std::chrono::high_resolution_clock> s_LastTime{
        std::chrono::high_resolution_clock::now()
    };

    // Calculate delta time
    const auto CURRENT_TIME = std::chrono::high_resolution_clock::now();
    g_DeltaTime = std::min(g_MaxDeltaTime, std::chrono::duration<double>(CURRENT_TIME - s_LastTime).count());
    s_LastTime = CURRENT_TIME;

	// Update elapsed time
    g_ElapsedTime += g_DeltaTime;

    // Calculate average fps
    g_FpsDeque.push_front(GetFps());
    if(static_cast<int>(g_FpsDeque.size()) > AMOUNT_OF_FRAMES_TO_AVERAGE)
        g_FpsDeque.pop_back();
    g_AverageFps = std::accumulate(g_FpsDeque.begin(), g_FpsDeque.end(), 0.0) / static_cast<int>(g_FpsDeque.size());
}
