#pragma once
#include "Renderer.h"
#include <glm/vec2.hpp>

namespace jul
{
	class Sprite;

	class SpriteRenderer final : public Renderer
	{
	public:
        SpriteRenderer(GameObject* parent, Sprite* sprite = nullptr, int renderLayer = 0,glm::ivec2 drawCell = {});

        void SetDrawCell(glm::ivec2 drawCell);
        void SetSprite(const Sprite* spritePtr);

        [[nodiscard]] const Sprite* GetSprite();

	private:

		void Render() const override;

        glm::ivec2 m_DrawCell;
        const Sprite* m_SpritePtr = nullptr; // Can be nullptr
	};
}

