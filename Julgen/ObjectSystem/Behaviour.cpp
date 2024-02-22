#include "Behaviour.h"

#include "GameObject.h"


jul::Behaviour::Behaviour(const std::string& name) :
Component{name},
m_Enabled{true}
{
	m_BehaviourPtrs.insert(this);
}

jul::Behaviour::~Behaviour()
{
	m_BehaviourPtrs.erase(this);
};


bool jul::Behaviour::IsActiveAndEnabled() const
{
	//return (m_Enabled && GetRawGameObject()->IsActiveInHierarchy());
	return false;
}

void jul::Behaviour::SetEnabled(bool enabled)
{
	if(enabled == m_Enabled)
		return;

	//if(GetRawGameObject()->IsActiveInHierarchy())
	//{
	//	if (enabled)
	//		OnEnable();
	//	else
	//		OnDisable();
	//}

	m_Enabled = enabled;
}

void jul::Behaviour::UpdateAll()
{
	for (auto iterator = m_BehaviourPtrs.begin(); iterator != m_BehaviourPtrs.end(); ++iterator)
	{
		if((*iterator)->IsActiveAndEnabled())
			(*iterator)->Update();
	}
}

void jul::Behaviour::LateUpdateAll()
{
	for (auto iterator = m_BehaviourPtrs.begin(); iterator != m_BehaviourPtrs.end(); ++iterator)
	{
		if ((*iterator)->IsActiveAndEnabled())
			(*iterator)->LateUpdate();
	}
}