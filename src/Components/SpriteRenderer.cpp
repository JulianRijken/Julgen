#include "SpriteRenderer.h"
#include "Sprite.h"
#include "Transform.h"
#include "RenderManager.h"

jul::SpriteRenderer::SpriteRenderer(GameObject* parentPtr, const Sprite* sprite, int renderLayer, const glm::ivec2& drawCell) :
    RenderComponent(parentPtr, "SpriteRenderer", renderLayer),
    m_DrawCell(drawCell)
{
    SetSprite(sprite);
}

void jul::SpriteRenderer::SetDrawCell(glm::ivec2 drawCell)
{
    m_DrawCell = drawCell;
}

void jul::SpriteRenderer::SetSprite(const Sprite* spritePtr) { m_SpritePtr = spritePtr; }

const jul::Sprite* jul::SpriteRenderer::GetSprite() const { return m_SpritePtr; }


void jul::SpriteRenderer::Render() const
{
	if(m_SpritePtr == nullptr)
		return;

    const glm::vec2& pos = GetTransform().WorldPosition();
    RenderManager::GetInstance().RenderTexture(
        m_SpritePtr->GetTexture(),
        pos,
        { m_SpritePtr->cellSize.x * m_DrawCell.x, m_SpritePtr->cellSize.y * m_DrawCell.y },
        m_SpritePtr->cellSize,
        m_SpritePtr->pixelsPerUnit,
        m_SpritePtr->pivot,
        m_FlipX,
        m_FlipY);
}
