#pragma once

namespace jul
{
	class Timing
	{
		friend class Julgen;

	public:

		static constexpr double FIXED_TIME_STEP = 1.0 / 60.0;

		static void SetTimeScale(double timeScale) { m_TimeScale = timeScale; }

		[[nodiscard]] static int GetFrameCount() { return m_FrameCount; }
		[[nodiscard]] static double GetDeltaTime() { return m_DeltaTime * m_TimeScale; }
		[[nodiscard]] static double GetFixedDeltaTime() { return FIXED_TIME_STEP; }
		[[nodiscard]] static double GetUnScaledDeltaTime() { return m_DeltaTime; }
		[[nodiscard]] static double GetTimeScale() { return m_TimeScale; }

	private:
		static void SetDeltaTime(double deltaTime) { m_DeltaTime = deltaTime; }
		static void AddToFrameCount() { m_FrameCount++; }

		inline static int m_FrameCount{ 0 };
		inline static double m_DeltaTime{ 0 };
		inline static double m_TimeScale{ 1.0f };
	};
}