#include "RenderComponent.h"

#include "RenderManager.h"


jul::RenderComponent::RenderComponent(GameObject* parentPtr, const std::string& name, int renderLayer) :
	Component( parentPtr, name ),
	m_RenderLayer( renderLayer )
{
	 RenderManager::g_RendererPtrs.insert(this);
}

jul::RenderComponent::~RenderComponent()
{
	RenderManager::g_RendererPtrs.erase(this);
}
