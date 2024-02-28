#include "SpriteRenderer.h"

#include "RenderManager.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "Transform.h"

jul::SpriteRenderer::SpriteRenderer(Sprite* sprite, int renderLayer, bool visible) :
	Renderer{ renderLayer, visible, "SpriteRenderer" },
	m_SpritePtr(sprite)
{}

void jul::SpriteRenderer::Render() const
{
	if(m_SpritePtr == nullptr)
		return;

	const glm::vec3& pos = Transform().WorldPosition();
	RenderManager::GetInstance().RenderTexture(*m_SpritePtr->GetTexture(), pos.x, pos.y);
}
