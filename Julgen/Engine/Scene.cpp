#include "Scene.h"

#include <iostream>

#include "GameObject.h"


using namespace jul;


Scene::Scene(const std::string& name) :
m_Name(name)
{}


void Scene::AddGameObjectToScene(std::shared_ptr<GameObject> object)
{
	m_GameObjectsInSceneSPtr.emplace_back(std::move(object));
}


void Scene::Update()
{
	for(const std::shared_ptr<GameObject>& gameObject : m_GameObjectsInSceneSPtr)
		gameObject->Update();
}

void Scene::Render() const
{
	for (const auto& object : m_GameObjectsInSceneSPtr)
		object->Render();
	
}

