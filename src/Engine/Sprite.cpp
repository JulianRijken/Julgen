#include "Sprite.h"

#include <algorithm>
#include "Texture2D.h"

jul::SpriteAnimation::SpriteAnimation(const std::vector<glm::ivec2>& cellFrames, const float framesPerSecond)
    : CELL_FRAMES(cellFrames),
      FRAME_COUNT(static_cast<int>(CELL_FRAMES.size())),
      FRAMES_PER_SECOND(framesPerSecond)
{}

const glm::ivec2& jul::SpriteAnimation::GetCellFromNormalizedTime(float time) const
{
    int frame = static_cast<int>(time * static_cast<float>(FRAME_COUNT));
    frame = std::clamp(frame, 0, FRAME_COUNT - 1);
    return CELL_FRAMES[frame];
}



jul::Sprite::Sprite(const Texture2D* texturePtr, int pixelsPerUnit, const glm::vec2& pivotAlpha, int rowCount,
                    int colCount, const std::map<std::string, SpriteAnimation>& animations) :

	PIXELS_PER_UNIT(pixelsPerUnit),
	PIVOT(pivotAlpha),
    CELL_SIZE(texturePtr->GetSize().x / colCount, texturePtr->GetSize().y / rowCount),
	TEXTURE_PTR(texturePtr),
	ANIMATIONS(animations)
{
}

const jul::SpriteAnimation* jul::Sprite::GetAnimation(const std::string& name) const
{
    assert(ANIMATIONS.contains(name) && "Animation does not exist");
    return &ANIMATIONS.at(name);
}

const jul::Texture2D& jul::Sprite::GetTexture() const
{
    return *TEXTURE_PTR;
}

