#pragma once
#include <deque>

#include "Julgen.h"

namespace jul
{
    class GameTime
    {
        // Required to call update
        friend void Julgen::RunOneFrame();

    private:
        // Privates put up front for the templates pfff

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

    public:
        GameTime() = delete;

        static void SetTimeScale(double timeScale) { g_TimeScale = timeScale; }

        // clang-format off

        template<typename Type = decltype(g_FrameCount)>
        [[nodiscard]] static inline Type GetFrameCount() { return g_FrameCount; }

        template<typename Type = decltype(g_ElapsedTime)>
        [[nodiscard]] static inline Type GetElapsedTime() { return g_ElapsedTime; }

        template<typename Type = decltype(1.0 / g_DeltaTime)>
        [[nodiscard]] static inline Type GetFps() { return 1.0 / g_DeltaTime; }

        template<typename Type = decltype(g_AverageFps)>
        [[nodiscard]] static inline Type GetSmoothFps() { return g_AverageFps; }

        template<typename Type = decltype(g_TimeScale)>
        [[nodiscard]] static inline Type GetTimeScale() { return g_TimeScale; }

        template<typename Type = decltype(g_DeltaTime)>
        [[nodiscard]] static inline Type GetUnScaledDeltaTime() { return static_cast<Type>(g_DeltaTime); }

        template<typename Type = decltype(g_DeltaTime * g_TimeScale)>
        [[nodiscard]] static inline Type GetDeltaTime() { return g_DeltaTime * g_TimeScale; }

        template<typename Type = decltype(g_FixedDeltaTime)>
        [[nodiscard]] static inline Type GetFixedDeltaTime() { return g_FixedDeltaTime; }

        // clang-format on
    };
}  // namespace jul
