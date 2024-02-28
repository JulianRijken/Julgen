#pragma once

#include <vector>
#include "glm/vec2.hpp"

namespace jul
{
	class Animation final
	{
	public:

		Animation(const std::vector<glm::ivec2>& cellFrames, float framesPerSecond = 0.0f);

		[[nodiscard]] const glm::ivec2& GetCellFromNormalizedTime(float time) const;
		[[nodiscard]] float GetFramesPerSecond() const { return m_FramesPerSecond;  }
		[[nodiscard]] const std::vector<glm::ivec2>& GetCellFrames() const;

		void SetFramesPerSecond(float framesPerSecond);

	private:
		float m_FramesPerSecond;

		const std::vector<glm::ivec2> CELL_FRAMES{};
		const int FRAME_COUNT{};
	};
}
