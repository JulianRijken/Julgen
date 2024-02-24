#include "Renderer.h"

#include "RenderManager.h"


jul::Renderer::Renderer(int renderLayer, bool visible, const std::string& name) :
	Component{ name },
	m_Visible{ visible },
	m_RenderLayer{ renderLayer }
{
	 RenderManager::s_GlobalRendererPtrs.insert(this);
}

jul::Renderer::~Renderer()
{
	RenderManager::s_GlobalRendererPtrs.erase(this);
}
