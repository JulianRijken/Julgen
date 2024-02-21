#include "RenderComponent.h"


jul::RenderComponent::RenderComponent(int renderLayer, bool visible, const std::string& name) :
	Component{ name },
	m_Visible{ visible },
	m_RenderLayer{ renderLayer }
{
}
