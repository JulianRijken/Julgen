#include "Component.h"
#include "GameObject.h"


jul::Component::Component(const std::string& name) :
	Object{ name }
{

}

jul::Transform& jul::Component::GetTransform() const
{
	return m_ParentGameObjectPtr->GetTransform();
}
