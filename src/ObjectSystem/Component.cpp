#include "Component.h"
#include "GameObject.h"


jul::Component::Component(GameObject* parentPtr, const std::string& name) :
	Object(name),
	m_ParentGameObjectPtr(parentPtr)
{
	assert(parentPtr);
}

jul::Transform& jul::Component::GetTransform() const { return m_ParentGameObjectPtr->GetTransform(); }

bool jul::Component::IsEnabledAndActive() const { return m_IsEnabled and m_ParentGameObjectPtr->IsActiveInHierarchy(); }

void jul::Component::SetEnabled(bool enabled) { m_IsEnabled = enabled; }
