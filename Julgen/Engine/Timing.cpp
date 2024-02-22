#include "Timing.h"

void jul::Time::UpdateDeltaTime()
{
	const auto currentTime = std::chrono::high_resolution_clock::now();
	m_DeltaTime = std::chrono::duration<double>(currentTime - m_LastTime).count();
	m_LastTime = currentTime;
}
