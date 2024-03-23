#pragma once

namespace jul
{
	class GlobalSettings
	{
	public:

		inline static constexpr char WINDOW_TITLE[] = "Julgen - Custom Engine By Julian Rijken";
        // inline static constexpr int RENDER_WIDTH{ 32 * 8 };
        // inline static constexpr int RENDER_HEIGHT{ 26 * 8 };

        inline static constexpr int RENDER_WIDTH{ 32 * 8 };
        inline static constexpr int RENDER_HEIGHT{ 26 * 4 };

        inline static constexpr int WINDOW_WIDTH{ RENDER_WIDTH * 4};
        inline static constexpr int WINDOW_HEIGHT{ RENDER_HEIGHT * 4};

        inline static constexpr float ASPECT_RATIO{WINDOW_WIDTH / WINDOW_HEIGHT};


		inline static constexpr double FIXED_TIME_STEP = 1.0 / 60.0;


#ifdef WIN32
		inline static constexpr bool SHOW_CONSOLE = true;
#endif

	};
}
