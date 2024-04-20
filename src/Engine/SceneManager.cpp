#include "SceneManager.h"

#include <ranges>


jul::GameObject* jul::SceneManager::AddGameObject(const std::string& name, const glm::vec3& position) const
{
    return m_ActiveScenePtr->AddGameObjectToScene(std::make_unique<GameObject>(name, position));
}


void jul::SceneManager::Update()
{
    for (const auto& scene : m_LoadedScenes | std::views::values)
        scene->Update();
}

void jul::SceneManager::LateUpdate()
{
    for(const auto& scene : m_LoadedScenes | std::views::values)
        scene->LateUpdate();
}

void jul::SceneManager::FixedUpdate()
{
	for (const auto& scene : m_LoadedScenes | std::views::values)
		scene->FixedUpdate();
}

void jul::SceneManager::CleanupGameObjects()
{
	for (const auto& scene : m_LoadedScenes | std::views::values)
		scene->CleanupGameObjects();
}

