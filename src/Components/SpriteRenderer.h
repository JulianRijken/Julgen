#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H
#include <glm/vec2.hpp>

#include "RenderComponent.h"

namespace jul
{
	struct Sprite;

	class SpriteRenderer final : public RenderComponent
	{
	public:
        SpriteRenderer(GameObject* parentPtr, const Sprite* spritePtr = nullptr, int renderLayer = 0,
                       const glm::ivec2& drawCell = {});

        void SetDrawCell(glm::ivec2 drawCell);
        void SetSprite(const Sprite* spritePtr);

        [[nodiscard]] const Sprite* GetSprite() const;

        bool m_FlipX{};  // NOLINT - C.131: Avoid trivial getters and setters
        bool m_FlipY{};  // NOLINT

    private:
        void Render() const override;

        glm::ivec2 m_DrawCell;
        const Sprite* m_SpritePtr = nullptr;
	};
}

#endif // SPRITERENDERER_H