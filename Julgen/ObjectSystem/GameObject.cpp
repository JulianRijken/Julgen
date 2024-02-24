#include "GameObject.h"

#include <iostream>

#include "Transform.h"


jul::GameObject::GameObject(const std::string& name, const glm::vec3& position) :
	Object(name),
	m_Transform(position)
{}


void jul::GameObject::Destroy()
{
	Object::Destroy();
	 
	for (const auto& component : m_Components)
		component->Destroy();
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

