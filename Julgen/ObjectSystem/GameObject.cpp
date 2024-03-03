#include "GameObject.h"
#include "Transform.h"


jul::GameObject::GameObject(const std::string& name, const glm::vec3& position) :
	Object(name)
{
	m_TransformPtr = reinterpret_cast<Transform*>(m_Components.emplace_back(std::make_unique<Transform>(position)).get());
	m_TransformPtr->m_ParentGameObjectPtr = this;
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


void jul::GameObject::Cleanup()
{
	for (auto iterator = m_Components.begin(); iterator != m_Components.end();)
	{
		if ((*iterator)->IsBeingDestroyed())
			iterator = m_Components.erase(iterator);
		else
			iterator++;
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


