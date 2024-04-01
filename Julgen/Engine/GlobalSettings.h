#pragma once

namespace jul
{
	class GlobalSettings
	{
	public:
        inline static constexpr char WINDOW_TITLE[] = "Julgen - Custom Engine by Julian Rijken";
        inline static constexpr int RENDER_WIDTH{ 32 * 8 * 4 };
        inline static constexpr int RENDER_HEIGHT{ 26 * 8 * 4};


        inline static constexpr int WINDOW_WIDTH{ RENDER_WIDTH};
        inline static constexpr int WINDOW_HEIGHT{ RENDER_HEIGHT};

        inline static constexpr float ASPECT_RATIO{WINDOW_WIDTH / WINDOW_HEIGHT};


		inline static constexpr double FIXED_TIME_STEP = 1.0 / 60.0;


#ifdef WIN32
		inline static constexpr bool SHOW_CONSOLE = true;
#endif

	};
}
