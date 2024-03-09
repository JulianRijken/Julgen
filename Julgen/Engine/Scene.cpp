#include "Scene.h"

#include <algorithm>
#include <execution>
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

void Scene::CleanupGameObjects()
{
	// We propagate the destroy !only at the end of the frame!
	// This is very important:
	// - If we propagate the destroy immediately, we might end up destroying a game object whos parent has changes after calling the destroy
	for (const std::unique_ptr<GameObject>& gameObject : m_GameObjectsInSceneSPtr)
	{
		if(gameObject->IsBeingDestroyed())
			gameObject->PropagateDestroy();
	}

	// Clean up individual components from game objects
	for (const std::unique_ptr<GameObject>& gameObject : m_GameObjectsInSceneSPtr)
	{
		if (gameObject->IsBeingDestroyed())
			gameObject->CleanupComponents();
	}

	// Remove all game objects that are set to be destroyed
	for (auto iterator = m_GameObjectsInSceneSPtr.begin(); iterator != m_GameObjectsInSceneSPtr.end();)
	{
		if ((*iterator)->IsBeingDestroyed())
			iterator = m_GameObjectsInSceneSPtr.erase(iterator);
		else
			iterator++;
	}
}

