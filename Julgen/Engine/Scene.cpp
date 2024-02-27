#include "Scene.h"

#include <iostream>

#include "GameObject.h"


using namespace jul;


Scene::Scene(const std::string& name) :
m_Name(name)
{}


GameObject* Scene::AddGameObjectToScene(std::unique_ptr<GameObject>&& object)
{
	m_GameObjectsInSceneSPtr.emplace_back(std::move(object));
	return m_GameObjectsInSceneSPtr.back().get();
}

void Scene::Update() const
{
	for(const std::unique_ptr<GameObject>& gameObject : m_GameObjectsInSceneSPtr)
		gameObject->Update();
}

void Scene::LateUpdate() const
{
	for (const std::unique_ptr<GameObject>& gameObject : m_GameObjectsInSceneSPtr)
		gameObject->LateUpdate();
}

void Scene::FixedUpdate() const
{
	for (const std::unique_ptr<GameObject>& gameObject : m_GameObjectsInSceneSPtr)
		gameObject->FixedUpdate();
}

void Scene::Cleanup()
{
	// Cleanup all components from the game objects
	for (const std::unique_ptr<GameObject>& gameObject : m_GameObjectsInSceneSPtr)
		gameObject->Cleanup();

	// Remove all game objects that are set to be destroyed
	for (auto iterator = m_GameObjectsInSceneSPtr.begin(); iterator != m_GameObjectsInSceneSPtr.end();)
	{
		if ((*iterator)->IsBeingDestroyed())
			iterator = m_GameObjectsInSceneSPtr.erase(iterator);
		else
			iterator++;
	}
}

