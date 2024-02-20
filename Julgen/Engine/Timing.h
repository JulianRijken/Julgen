#pragma once


namespace jul
{
	class Timing
	{
		friend class Julgen;

	public:
		static void SetTimeScale(float timeScale);

		[[nodiscard]] static float GetDeltaTime() { return m_DeltaTime * m_TimeScale; }
		[[nodiscard]] static float GetUnScaledDeltaTime() { return m_DeltaTime; }
		[[nodiscard]] static int GetFrameCount() { return m_FrameCount; }
		[[nodiscard]] static float GetTimeScale() { return m_TimeScale; }

	private:
		static void SetDeltaTime(float deltaTime) { m_DeltaTime = deltaTime; }
		static void AddToFrameCount() { m_FrameCount++; }

		inline static float m_DeltaTime{ 0 };
		inline static int m_FrameCount{ 0 };
		inline static float m_TimeScale{ 1.0f };
	};
}