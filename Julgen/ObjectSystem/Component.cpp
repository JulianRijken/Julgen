#include "Component.h"
#include "GameObject.h"


jul::Transform& jul::Component::Transform() const
{
	return m_ParentGameObjectPtr->GetTransform(); 
}

jul::Component::Component(const std::string& name) :
	Object{ name }
{}
