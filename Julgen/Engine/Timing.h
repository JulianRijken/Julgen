#pragma once
#include <chrono>
#include "GlobalSettings.h"

namespace jul
{
	class Time
	{
		friend class Julgen;

	public:

		static void SetTimeScale(double timeScale) { m_TimeScale = timeScale; }

		[[nodiscard]] static int    GetFrameCount() { return m_FrameCount; }
		[[nodiscard]] static double GetTimeScale() { return m_TimeScale; }
		[[nodiscard]] static double GetUnScaledDeltaTime() { return m_DeltaTime; }
		[[nodiscard]] static double GetDeltaTime() { return m_DeltaTime * m_TimeScale; }
		[[nodiscard]] static double GetFixedDeltaTime() { return GlobalSettings::FIXED_TIME_STEP; }

	private:
	
		static void UpdateDeltaTime(); 	// Should be called at the beginning of every game loop tick
		static void AddToFrameCount() { m_FrameCount++; }

		inline static int m_FrameCount{ 0 };
		inline static double m_DeltaTime{ 0 };
		inline static double m_TimeScale{ 1.0f };
		inline static std::chrono::time_point<std::chrono::steady_clock> m_LastTime;
	};
}
