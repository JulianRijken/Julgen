#include "Component.h"
#include "GameObject.h"


jul::Component::Component(GameObject* parent, const std::string& name) :
	Object(name),
	m_ParentGameObjectPtr(parent)
{
	assert(parent);
}

jul::Transform& jul::Component::Transform() const
{
	return m_ParentGameObjectPtr->GetTransform();
}
