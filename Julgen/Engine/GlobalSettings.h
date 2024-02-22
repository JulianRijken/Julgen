#pragma once

namespace jul
{
	class GlobalSettings
	{
	public:

		inline static constexpr char WINDOW_TITLE[] = "Custom window title";
		inline static constexpr int RENDER_WIDTH{ 640 };
		inline static constexpr int RENDER_HEIGHT{ 480 };

		inline static constexpr int WINDOW_WIDTH{ RENDER_WIDTH };
		inline static constexpr int WINDOW_HEIGHT{ RENDER_HEIGHT };

		inline static constexpr char CONTENT_PATH[] = "Content/";

		inline static constexpr double FIXED_TIME_STEP = 1.0 / 60.0;


#ifdef WIN32
		inline static constexpr bool SHOW_CONSOLE = true;
#endif

	};
}
