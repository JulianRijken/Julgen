#include "Component.h"
#include "GameObject.h"


jul::Component::Component(GameObject* parentPtr, const std::string& name) :
	Object(name),
	m_ParentGameObjectPtr(parentPtr)
{
	assert(parentPtr);
}

jul::Transform& jul::Component::GetTransform() const { return m_ParentGameObjectPtr->GetTransform(); }
