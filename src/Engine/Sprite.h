#pragma once
#include <map>
#include <string>
#include <vector>
#include <glm/vec2.hpp>


namespace jul
{
	class Texture2D;

    struct SpriteAnimation final
    {
        SpriteAnimation(const std::vector<glm::ivec2>& cellFrames, const float framesPerSecond = 0.0f);

        [[nodiscard]] const glm::ivec2& GetCellFromNormalizedTime(float time) const;

        const std::vector<glm::ivec2> CELL_FRAMES{};
        const int FRAME_COUNT{};
        const float FRAMES_PER_SECOND;
    };

    struct Sprite final
	{
		Sprite(const Texture2D* texturePtr, int pixelsPerUnit = 100, const glm::vec2& pivotAlpha = {},
               int rowCount = 1, int colCount = 1,const std::map<std::string, SpriteAnimation>& animations = {});
        
        [[nodiscard]] const SpriteAnimation* GetAnimation(const std::string& name) const;
        [[nodiscard]] const Texture2D& GetTexture() const;

        const int PIXELS_PER_UNIT;
		const glm::vec2 PIVOT;
		const glm::ivec2 CELL_SIZE;
		const Texture2D* TEXTURE_PTR;
        const std::map<std::string, SpriteAnimation> ANIMATIONS;
	};
}
