#include "SceneManager.h"

#include <ranges>

void jul::SceneManager::UnloadScene(const std::string& sceneName) { m_LoadedScenes.erase(sceneName); }

void jul::SceneManager::Destroy() { m_LoadedScenes.clear(); }

jul::GameObject* jul::SceneManager::AddGameObject(const std::string& name, const glm::vec3& position) const
{
    return m_PrimaryScenePtr->AddGameObject(name, position);
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

