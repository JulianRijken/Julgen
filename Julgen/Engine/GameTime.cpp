#include "GameTime.h"

#include <numeric>

void jul::GameTime::Update()
{
	// Calculate delta time
    const auto CURRENT_TIME = std::chrono::high_resolution_clock::now();
    s_DeltaTime = std::min(MAX_DELTA_TIME, std::chrono::duration<double>(CURRENT_TIME - s_LastTime).count());
    s_LastTime = CURRENT_TIME;

	// Update elapsed time
	s_ElapsedTime += s_DeltaTime;

	// Calculate average fps
	s_FpsDeque.push_front(GetFps());
	if (static_cast<int>(s_FpsDeque.size()) > AMOUNT_OF_FRAMES_TO_AVERAGE)
		s_FpsDeque.pop_back();
	s_AverageFps = std::accumulate(s_FpsDeque.begin(), s_FpsDeque.end(), 0.0) / static_cast<int>(s_FpsDeque.size());
}
