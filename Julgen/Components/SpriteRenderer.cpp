#include "SpriteRenderer.h"

#include "Renderer.h"
#include "ResourceManager.h"
#include "Transform.h"

jul::SpriteRenderer::SpriteRenderer(const std::string& fileName, int renderLayer, bool visible, const std::string& name) :
	RenderComponent{ renderLayer, visible, name }
{
	m_TextureSPtr = ResourceManager::GetInstance().LoadTexture(fileName);
}

void jul::SpriteRenderer::Render() const
{
	const auto& pos = GetTransform().GetPosition();
	Renderer::GetInstance().RenderTexture(*m_TextureSPtr, pos.x, pos.y);
}
