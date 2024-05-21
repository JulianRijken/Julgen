#include "GameObject.h"
#include "Transform.h"

jul::GameObject::GameObject(const std::string& name, Scene* scene, const glm::vec3& position) :
    Object(name),
    m_ScenePtr(scene)
{
    m_TransformPtr = dynamic_cast<Transform*>(m_Components.emplace_back(std::make_unique<Transform>(this,position)).get());
}

bool jul::GameObject::IsActiveInHierarchy()
{
    if(m_ActiveDirty)
        UpdateActiveInHierarchy();

    return m_ActiveInHierarchy;
}

void jul::GameObject::SetActive(bool active)
{
    if(active == m_ActiveSelf)
        return;

    m_ActiveSelf = active;
    SetActiveDirty();
}


void jul::GameObject::Destroy()
{
	Object::Destroy();

	for (const auto& component : m_Components)
		component->Destroy();
}

void jul::GameObject::PropagateDestroy() const
{
	for (const Transform* child : m_TransformPtr->GetChildren())
	{
		if(child->GetGameObject()->IsBeingDestroyed())
			continue;

		child->GetGameObject()->Destroy();
		child->GetGameObject()->PropagateDestroy();
	}
}

void jul::GameObject::CleanupComponents()
{
	for (auto iterator = m_Components.begin(); iterator != m_Components.end();)
	{
		if ((*iterator)->IsBeingDestroyed())
			iterator = m_Components.erase(iterator);
		else
			++iterator;
	}
}


void jul::GameObject::Update() const
{
	for (const auto& component : m_Components)
		component->Update();	
}

void jul::GameObject::LateUpdate() const
{
	for (const auto& component : m_Components)
		component->LateUpdate();
}

void jul::GameObject::FixedUpdate() const
{
	for (const auto& component : m_Components)
		component->FixedUpdate();
}

void jul::GameObject::UpdateActiveInHierarchy()
{
    auto* parentPtr = m_TransformPtr->GetParent();

    if(parentPtr == nullptr)
        m_ActiveInHierarchy = m_ActiveSelf;
    else
        m_ActiveInHierarchy = m_ActiveSelf and parentPtr->GetGameObject()->IsActiveInHierarchy();

    m_ActiveDirty = false;
}

void jul::GameObject::SetActiveDirty()
{
    m_ActiveDirty = true;

    for(Transform* childPtr : m_TransformPtr->GetChildren())
        if(not childPtr->GetGameObject()->m_ActiveDirty)
            childPtr->GetGameObject()->SetActiveDirty();
}
