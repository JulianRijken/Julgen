#pragma once
#include "RenderComponent.h"
#include <glm/vec2.hpp>

namespace jul
{
	struct Sprite;

	class SpriteRenderer final : public RenderComponent
	{
	public:
        SpriteRenderer(GameObject* parent, Sprite* sprite = nullptr, int renderLayer = 0,glm::ivec2 drawCell = {});

        void SetDrawCell(glm::ivec2 drawCell);
        void SetSprite(const Sprite* spritePtr);

        [[nodiscard]] const Sprite* GetSprite();

        bool FlipX{}; // NOLINT - C.131: Avoid trivial getters and setters
        bool FlipY{}; // NOLINT

        void SetFlipX(bool newFlipX);

    private:
        void Render() const override;

        glm::ivec2 m_DrawCell;
        const Sprite* m_SpritePtr = nullptr;
	};
}

