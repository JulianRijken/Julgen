#ifndef SPRITE_H
#define SPRITE_H
#include <map>
#include <string>
#include <vector>
#include <glm/vec2.hpp>


namespace jul
{
	class Texture2D;

    struct SpriteAnimation final
    {
        SpriteAnimation(const std::vector<glm::ivec2>& cellFrames, int framesPerSecond = 0.0f);

        [[nodiscard]] const glm::ivec2& GetCellFromNormalizedTime(float time) const;

        std::vector<glm::ivec2> cellFrames{};
        int frameCount{};
        int framesPerSecond;
    };

    struct Sprite final
	{
        Sprite(Texture2D* texturePtr, int pixelsPerUnit = 100, const glm::vec2& pivotAlpha = {}, int rowCount = 1,
               int colCount = 1, const std::map<std::string, SpriteAnimation>& animations = {});

        [[nodiscard]] const SpriteAnimation* GetAnimation(const std::string& name) const;
        [[nodiscard]] const Texture2D& GetTexture() const;

        int pixelsPerUnit;
        glm::vec2 pivot;
        glm::ivec2 cellSize;
        Texture2D* texturePtr;
        std::map<std::string, SpriteAnimation> animations;
    };
}
#endif // SPRITE_H