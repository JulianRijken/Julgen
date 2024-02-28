#pragma once
#include <map>
#include <memory>
#include <string>

#include "Animation.h"
#include "glm/vec2.hpp"

namespace jul
{
	class Texture2D;

	class Sprite final
	{

	public:

		Sprite(const Texture2D* texturePtr, float pixelsPerUnit, const glm::vec2& pivotAlpha = {},
		       int rowCount = 1, int colCount = 1,const std::map<std::string, Animation>& animations = {});

		~Sprite() = default;

		Sprite(Sprite&& other) = delete;
		Sprite(const Sprite& other) = delete;
		Sprite& operator=(Sprite&& other) = delete;
		Sprite& operator=(const Sprite& other) = delete;

		[[nodiscard]] const Animation* GetAnimation(const std::string& name) const;
		[[nodiscard]] const Texture2D* GetTexture() const;


	private:

		const int ROW_COUNT;
		const int COL_COUNT;
		const float PIXELS_PER_UNIT;
		const glm::vec2 PIVOT;
		const glm::ivec2 CELL_SIZE;
		const Texture2D* TEXTURE_PTR;
		const std::map<std::string, Animation> ANIMATIONS;


	};
}
