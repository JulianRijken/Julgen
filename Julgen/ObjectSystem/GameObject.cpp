#include "GameObject.h"

#include <iostream>

#include "RenderComponent.h"


jul::GameObject::GameObject(const std::string& name) :
	Object(name)
{
}



void jul::GameObject::Update()
{
}

void jul::GameObject::Render() const
{
	// HORRIBLE
	// TODO:  Do I go with the render function to render function to render function or do It like I did for afterburner

	for (const std::shared_ptr<Component>& component : m_Components)
	{
		RenderComponent* castComponentPtr{ dynamic_cast<RenderComponent*>(component.get()) };

		if(castComponentPtr)
		castComponentPtr->Render();
	}
}


