#include "SpriteRenderer.h"

#include "RenderManager.h"
#include "ResourceManager.h"
#include "Transform.h"

jul::SpriteRenderer::SpriteRenderer(const std::string& fileName, int renderLayer, bool visible, const std::string& name) :
	Renderer{ renderLayer, visible, name }
{
	m_TextureSPtr = ResourceManager::GetInstance().LoadTexture(fileName);
}

void jul::SpriteRenderer::Render() const
{
	const glm::vec3& pos = Transform().WorldPosition();
	RenderManager::GetInstance().RenderTexture(*m_TextureSPtr, pos.x, pos.y);
}
