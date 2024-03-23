#include "SpriteRenderer.h"
#include "Sprite.h"
#include "Transform.h"
#include "RenderManager.h"


jul::SpriteRenderer::SpriteRenderer(GameObject* parentPtr, Sprite* sprite, int renderLayer,glm::ivec2 drawCell) :
      RenderComponent(parentPtr,"SpriteRenderer",renderLayer),
    m_DrawCell(drawCell)
{
    SetSprite(sprite);
}

void jul::SpriteRenderer::SetDrawCell(glm::ivec2 drawCell)
{
    m_DrawCell = drawCell;
}

void jul::SpriteRenderer::SetSprite(const Sprite* spritePtr)
{
    m_SpritePtr = spritePtr;
}


const jul::Sprite* jul::SpriteRenderer::GetSprite()
{
    return m_SpritePtr;
}

void jul::SpriteRenderer::Render() const
{
	if(m_SpritePtr == nullptr)
		return;

    const glm::vec2& pos = Transform().WorldPosition();
    RenderManager::GetInstance().RenderTexture(
        m_SpritePtr->GetTexture(),
        pos,
        {m_SpritePtr->CELL_SIZE.x * float(m_DrawCell.x), m_SpritePtr->CELL_SIZE.y * float(m_DrawCell.y)},
        m_SpritePtr->CELL_SIZE,
        m_SpritePtr->PIXELS_PER_UNIT,
        m_SpritePtr->PIVOT,
        m_FlipX,
        m_FlipY
        );
}
