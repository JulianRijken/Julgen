#pragma once

namespace jul
{
	class GlobalSettings
	{
	public:

		inline static constexpr char WINDOW_TITLE[] = "Custom window title";
		inline static constexpr int RENDER_WIDTH{ 320 };
		inline static constexpr int RENDER_HEIGHT{ 224 };

		inline static constexpr int WINDOW_WIDTH{ RENDER_WIDTH * 4 };
		inline static constexpr int WINDOW_HEIGHT{ RENDER_HEIGHT * 4 };

		inline static constexpr char CONTENT_PATH[] = "Content/";

#ifdef WIN32
		inline static constexpr bool SHOW_CONSOLE = true;
#endif

	};
}
