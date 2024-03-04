#pragma once
#include <chrono>
#include <deque>

#include "GlobalSettings.h"


namespace jul
{
	class GameTime
	{

	public:

		static void SetTimeScale(double timeScale) { s_TimeScale = timeScale; }

		[[nodiscard]] static int    GetFrameCount()        { return s_FrameCount; }
		[[nodiscard]] static double GetElapsedTime()	   { return s_ElapsedTime; }
		[[nodiscard]] static double GetFps()               { return 1.0 / s_DeltaTime; }
		[[nodiscard]] static double GetSmoothFps()         { return s_AverageFps; }
		[[nodiscard]] static double GetTimeScale()         { return s_TimeScale; }
		[[nodiscard]] static double GetUnScaledDeltaTime() { return s_DeltaTime; }
		[[nodiscard]] static double GetDeltaTime()         { return s_DeltaTime * s_TimeScale; }
		[[nodiscard]] static float  GetDeltaTimeF()        { return static_cast<float>(s_DeltaTime * s_TimeScale); }
		[[nodiscard]] static double GetFixedDeltaTime()    { return GlobalSettings::FIXED_TIME_STEP; }
		[[nodiscard]] static float  GetFixedDeltaTimeF()   { return static_cast<float>(GlobalSettings::FIXED_TIME_STEP); }


		// TODO: So in this case I also removed friend class to Julgen. Is this really better? Update() AddToFrameCount()

		// Should only be called once per frame by the engine
		static void Update();
		// Should only be called once per frame by the engine
		static void AddToFrameCount() { s_FrameCount++; }

	private:

		inline static int s_FrameCount{ 0 };
		inline static double s_ElapsedTime{ 0 };
		inline static double s_DeltaTime{ 0.0 };
		inline static double s_TimeScale{ 1.0 };
		inline static std::chrono::time_point<std::chrono::steady_clock> s_LastTime{ std::chrono::high_resolution_clock::now() };

		inline static std::deque<double> s_FpsDeque{};
		inline static double s_AverageFps{ 0 };
		inline static constexpr int AMOUNT_OF_FRAMES_TO_AVERAGE{ 60 };
		inline static constexpr double MAX_DELTA_TIME{ 1.0 / 30.0 };
	};
}

