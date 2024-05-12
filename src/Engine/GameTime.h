#pragma once
#include <deque>

namespace jul
{
    class GameTime
    {
        friend class Julgen;

    public:
        GameTime() = delete;

        static void SetTimeScale(double timeScale) { g_TimeScale = timeScale; }

        [[nodiscard]] static int GetFrameCount() { return g_FrameCount; }

        [[nodiscard]] static double GetElapsedTime() { return g_ElapsedTime; }

        [[nodiscard]] static float GetElapsedTimeF() { return static_cast<float>(g_ElapsedTime); }

        [[nodiscard]] static double GetFps() { return 1.0 / g_DeltaTime; }

        [[nodiscard]] static double GetSmoothFps() { return g_AverageFps; }

        [[nodiscard]] static double GetTimeScale() { return g_TimeScale; }

        [[nodiscard]] static double GetUnScaledDeltaTime() { return g_DeltaTime; }

        [[nodiscard]] static double GetDeltaTime() { return g_DeltaTime * g_TimeScale; }

        [[nodiscard]] static float GetDeltaTimeF() { return static_cast<float>(g_DeltaTime * g_TimeScale); }

        [[nodiscard]] static double GetFixedDeltaTime() { return g_FixedDeltaTime; }

        [[nodiscard]] static float GetFixedDeltaTimeF() { return static_cast<float>(g_FixedDeltaTime); }


    private:
        static void Update();

        static void AddToFrameCount() { g_FrameCount++; }

        inline static double g_MaxDeltaTime{ 1.0 / 30.0 };
        inline static double g_FixedDeltaTime{ 1.0 / 120.0 };

        inline static int g_FrameCount{ 0 };
        inline static double g_ElapsedTime{ 0 };
        inline static double g_DeltaTime{ 0.0 };
        inline static double g_TimeScale{ 1.0 };

        inline static std::deque<double> g_FpsDeque{};
        inline static double g_AverageFps{ 0 };
        inline static constexpr int AMOUNT_OF_FRAMES_TO_AVERAGE{ 60 };
    };
}  // namespace jul
