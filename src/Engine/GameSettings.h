#include <string>

#pragma once

namespace jul
{
    class GameSettings
    {
    public:
        static float GetAspectRatio() { return static_cast<float>(s_WindowWidth) / static_cast<float>(s_WindowHeight); }

        inline static std::string s_WindowTitle = "Julgen - Custom Engine by Julian Rijken";
        inline static int s_RenderWidth{ 1280 };
        inline static int s_RenderHeight{ 720 };
        inline static int s_WindowWidth{ 1280 };
        inline static int s_WindowHeight{ 720 };
        inline static bool m_ShowConsole{ true };
    };
}
