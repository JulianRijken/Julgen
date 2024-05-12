#include "Sprite.h"

#include <algorithm>
#include "Texture2D.h"

jul::SpriteAnimation::SpriteAnimation(const std::vector<glm::ivec2>& cellFrames, int framesPerSecond) :
    cellFrames(cellFrames),
    frameCount(static_cast<int>(cellFrames.size())),
    framesPerSecond(framesPerSecond)
{}

const glm::ivec2& jul::SpriteAnimation::GetCellFromNormalizedTime(float time) const
{
    int frame = static_cast<int>(time * static_cast<float>(frameCount));
    frame = std::clamp(frame, 0, frameCount - 1);
    return cellFrames[frame];
}

jul::Sprite::Sprite(Texture2D* texturePtr, int pixelsPerUnit, const glm::vec2& pivotAlpha, int rowCount, int colCount,
                    const std::map<std::string, SpriteAnimation>& animations) :

    pixelsPerUnit(pixelsPerUnit),
    pivot(pivotAlpha),
    cellSize(texturePtr->GetSize().x / colCount, texturePtr->GetSize().y / rowCount),
    texturePtr(texturePtr),
    animations(animations)
{
}

const jul::SpriteAnimation* jul::Sprite::GetAnimation(const std::string& name) const
{
    assert(animations.contains(name) && "Animation does not exist");
    return &animations.at(name);
}

const jul::Texture2D& jul::Sprite::GetTexture() const { return *texturePtr; }
