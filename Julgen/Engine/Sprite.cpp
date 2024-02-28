#include "Sprite.h"

#include "ResourceManager.h"
#include "Texture2D.h"

jul::Sprite::Sprite(const Texture2D* texturePtr, float pixelsPerUnit, const glm::vec2& pivotAlpha, int rowCount,
                    int colCount, const std::map<std::string, Animation>& animations) :

	ROW_COUNT(std::max(1, rowCount)),
	COL_COUNT(std::max(1, colCount)),
	PIXELS_PER_UNIT(pixelsPerUnit),
	PIVOT(pivotAlpha),
	CELL_SIZE(texturePtr->GetSize().x / COL_COUNT, texturePtr->GetSize().y / ROW_COUNT),
	TEXTURE_PTR(texturePtr),
	ANIMATIONS(animations)
{
}

const jul::Animation* jul::Sprite::GetAnimation(const std::string& name) const
{
	return ANIMATIONS.contains(name) ? &ANIMATIONS.at(name) : nullptr;
}

const jul::Texture2D* jul::Sprite::GetTexture() const
{
	return TEXTURE_PTR;
}
