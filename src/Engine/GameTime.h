#ifndef GAMETIME_H
#define GAMETIME_H
#include <deque>

#include "Julgen.h"

namespace jul
{
    class GameTime final
    {
        // Required to call update
        friend void Julgen::RunOneFrame();

    private:
        // Privates put up front for the templates

        static void Update();

        static void AddToFrameCount() { g_FrameCount++; }

        inline static constexpr double MAX_DELTA_TIME{ 1.0 / 30.0 };
        inline static double g_FixedDeltaTime{ 1.0 / 120.0 };

        inline static int g_FrameCount{ 0 };
        inline static double g_ElapsedTime{ 0 };
        inline static double g_DeltaTime{ 0.0 };
        inline static double g_TimeScale{ 1.0 };

        inline static int g_FramesCountedSinceLastAverage{ 0 };
        inline static double g_FpsUpdateTimer{ 0 };
        inline static double g_AccumulatedDeltaTime{ 0 };
        inline static double g_AverageFps{ 0 };
        inline static constexpr double SMOOTH_FPS_UPDATE_RATE{ 1.0 };

    public:
        GameTime() = delete;

        static void SetTimeScale(double timeScale) { g_TimeScale = timeScale; }

        // clang-format off

        template<typename Type = decltype(g_FrameCount)>
        [[nodiscard]] static inline Type GetFrameCount() { return static_cast<Type>(g_FrameCount); }

        template<typename Type = decltype(g_ElapsedTime)>
        [[nodiscard]] static inline Type GetElapsedTime() { return static_cast<Type>(g_ElapsedTime); }

        template<typename Type = decltype(1.0 / g_DeltaTime)>
        [[nodiscard]] static inline Type GetFps() { return static_cast<Type>(1.0 / g_DeltaTime); }

        template<typename Type = decltype(g_AverageFps)>
        [[nodiscard]] static inline Type GetSmoothFps() { return static_cast<Type>(g_AverageFps); }

        template<typename Type = decltype(g_TimeScale)>
        [[nodiscard]] static inline Type GetTimeScale() { return static_cast<Type>(g_TimeScale); }

        template<typename Type = decltype(g_DeltaTime)>
        [[nodiscard]] static inline Type GetUnScaledDeltaTime() { return static_cast<Type>(g_DeltaTime); }

        template<typename Type = decltype(g_DeltaTime * g_TimeScale)>
        [[nodiscard]] static inline Type GetDeltaTime() { return static_cast<Type>(g_DeltaTime * g_TimeScale); }

        template<typename Type = decltype(g_FixedDeltaTime)>
        [[nodiscard]] static inline Type GetFixedDeltaTime() { return static_cast<Type>(g_FixedDeltaTime); }

        // clang-format on
    };
}  // namespace jul
#endif // GAMETIME_H