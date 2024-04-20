#include "RenderComponent.h"

#include "RenderManager.h"


jul::RenderComponent::RenderComponent(GameObject* parent, const std::string& name, int renderLayer) :
	Component( parent, name ),
	m_RenderLayer( renderLayer )
{
	 RenderManager::s_GlobalRendererPtrs.insert(this);
}

jul::RenderComponent::~RenderComponent()
{
	RenderManager::s_GlobalRendererPtrs.erase(this);
}
