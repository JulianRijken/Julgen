
#pragma once

#include <string>

namespace jul
{
    class GameSettings
    {
    public:
        static float GetAspectRatio() { return static_cast<float>(g_WindowWidth) / static_cast<float>(g_WindowHeight); }

        inline static std::string g_WindowTitle = "Julgen - Custom Engine by Julian Rijken";
        inline static int g_RenderWidth{ 1280 };
        inline static int g_RenderHeight{ 720 };
        inline static int g_WindowWidth{ 1280 };
        inline static int g_WindowHeight{ 720 };
        inline static bool g_ShowConsole{ true };
    };
}
