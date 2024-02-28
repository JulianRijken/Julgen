#include "Animation.h"

#include <algorithm>


jul::Animation::Animation(const std::vector<glm::ivec2>& cellFrames, float framesPerSecond) :
	m_FramesPerSecond(framesPerSecond),
	CELL_FRAMES(cellFrames),
	FRAME_COUNT(static_cast<int>(CELL_FRAMES.size()))
{}

const glm::ivec2& jul::Animation::GetCellFromNormalizedTime(float time) const
{
	int frame = static_cast<int>(time * static_cast<float>(FRAME_COUNT));
	frame = std::clamp(frame, 0, FRAME_COUNT - 1);
	return CELL_FRAMES[frame];
}

const std::vector<glm::ivec2>& jul::Animation::GetCellFrames() const
{
	return CELL_FRAMES;
}

void jul::Animation::SetFramesPerSecond(float framesPerSecond)
{
	m_FramesPerSecond = framesPerSecond;
}
