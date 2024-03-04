#include "Renderer.h"

#include "RenderManager.h"


jul::Renderer::Renderer(GameObject* parent, const std::string& name, int renderLayer) :
	Component( parent, name ),
	m_RenderLayer( renderLayer )
{
	 RenderManager::s_GlobalRendererPtrs.insert(this);
}

jul::Renderer::~Renderer()
{
	RenderManager::s_GlobalRendererPtrs.erase(this);
}
