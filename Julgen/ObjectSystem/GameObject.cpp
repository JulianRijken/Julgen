#include "GameObject.h"

#include <iostream>

#include "Transform.h"


jul::GameObject::GameObject(const std::string& name, const glm::vec3& position) :
	Object(name),
	m_Transform(position)
{
	m_Transform.m_ParentGameObjectPtr = this;
}


void jul::GameObject::Destroy()
{
	Object::Destroy();

	// Destroy components
	for (const std::unique_ptr<Component>& component : m_Components)
		component->Destroy();

	// Destroy children
	for (const Transform* childTransform : m_Transform.GetChildren())
		childTransform->GetGameObject()->Destroy();
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

