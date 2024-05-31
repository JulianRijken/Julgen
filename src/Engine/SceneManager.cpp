#include "SceneManager.h"

#include <fmt/format.h>


jul::GameObject* jul::SceneManager::AddGameObject(const std::string& name, const glm::vec3& position) const
{
    return m_PrimaryScenePtr->AddGameObject(name, position);
}

void jul::SceneManager::Update()
{
    for(const auto& scene : m_LoadedScenes)
        scene->Update();
}

void jul::SceneManager::LateUpdate()
{
    for(const auto& scene : m_LoadedScenes)
        scene->LateUpdate();
}

void jul::SceneManager::FixedUpdate()
{
    for(const auto& scene : m_LoadedScenes)
        scene->FixedUpdate();
}

void jul::SceneManager::Destroy()
{
    for(auto&& scene : m_LoadedScenes)
        scene->MarkForUnload();

    CleanupGameObjects();
    CleanupScenes();
}

void jul::SceneManager::MarkScenesForUnload()
{
    if(m_ScenesToLoad.empty())
        return;

    // If the first scene is set to override all scenes should be unloaded
    if(m_ScenesToLoad.front().second == SceneLoadMode::Override)
        for(auto&& scene : m_LoadedScenes)
            scene->MarkForUnload();
}

void jul::SceneManager::CleanupGameObjects()
{
    for(const auto& scene : m_LoadedScenes)
        scene->CleanupGameObjects();
}

void jul::SceneManager::CleanupScenes()
{
    for(auto iterator = m_LoadedScenes.begin(); iterator != m_LoadedScenes.end();)
        if((*iterator)->m_BeingUnloaded)
            iterator = m_LoadedScenes.erase(iterator);
        else
            ++iterator;
}

void jul::SceneManager::LoadScenesSetToLoad()
{
    if(m_ScenesToLoad.empty())
        return;

    for(auto&& [name, loadMode] : m_ScenesToLoad)
    {
        if(not m_SceneBinds.contains(name))
            throw std::runtime_error(fmt::format("Scene {} has not been binded when loading", name));

        auto newSceneUPtr = std::make_unique<Scene>(name);

        // There should only ever be one override when loading here
        if(loadMode == SceneLoadMode::Override)
            m_PrimaryScenePtr = newSceneUPtr.get();

        // Call load function
        m_SceneBinds[name](*newSceneUPtr);

        // Store scene
        m_LoadedScenes.emplace_back(std::move(newSceneUPtr));
    }

    m_ScenesToLoad.clear();
}
