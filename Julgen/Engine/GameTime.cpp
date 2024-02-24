#include "GameTime.h"
#include <numeric>


void jul::GameTime::Update()
{
	const auto currentTime = std::chrono::high_resolution_clock::now();
	s_DeltaTime = std::chrono::duration<double>(currentTime - s_LastTime).count();
	s_LastTime = currentTime;


	s_FpsDeque.push_front(GetFps());

	if (static_cast<int>(s_FpsDeque.size()) > AMOUNT_OF_FRAMES_TO_AVERAGE)
		s_FpsDeque.pop_back();

	s_AverageFps = std::accumulate(s_FpsDeque.begin(), s_FpsDeque.end(), 0.0) / static_cast<int>(s_FpsDeque.size());
}


