#include "SceneManager.h"

#include <fmt/format.h>

void jul::SceneManager::LoadScene(int id, SceneLoadMode loadMode)
{
    // Cancle load when first scene in scenesToLoad is override force
    if(not m_ScenesToLoad.empty() and m_ScenesToLoad.front().second == SceneLoadMode::OverrideForce)
        return;

    if(loadMode != SceneLoadMode::Additive)
        m_ScenesToLoad.clear();

    m_ScenesToLoad.emplace_back(id, loadMode);
}

jul::Scene* jul::SceneManager::FindScene(int id)
{
    for(const auto& scene : m_LoadedScenes)
        if(scene->m_Id == id)
            return scene.get();

    return nullptr;
}

void jul::SceneManager::BindScene(int id, std::function<void(Scene&)>&& sceneFunction)
{
    m_SceneBinds[id] = std::move(sceneFunction);
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
        scene->Unload();

    CleanupGameObjects();
    CleanupScenes();
}

void jul::SceneManager::ProcessScenes()
{
    MarkScenesForUnload();
    CleanupGameObjects();
    CleanupScenes();
    LoadScenesSetToLoad();

    // Our user actually loded a scene when loading a scene
    // So we start our whole cleanup process over again
    if(not m_ScenesToLoad.empty())
        ProcessScenes();
}


void jul::SceneManager::MarkScenesForUnload()
{
    if(m_ScenesToLoad.empty())
        return;

    // If the first scene is set to override all scenes should be unloaded
    if(m_ScenesToLoad.front().second != SceneLoadMode::Additive)
        for(auto&& scene : m_LoadedScenes)
            scene->Unload();
}


void jul::SceneManager::CleanupGameObjects()
{
    // We clean up all game objects
    for(const auto& scene : m_LoadedScenes)
    {
        scene->MoveGameObjectsAdded();
        scene->CleanupGameObjects();
    }

    // What if a game object has been added during the destroy?
    // We do another cleanup and move
    for(const auto& scene : m_LoadedScenes)
    {
        if(not scene->m_GameObjectsAdded.empty())
        {
            scene->MoveGameObjectsAdded();
            CleanupGameObjects();
        }
    }
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

    // We need a clear and copy as the vector might change during the loop
    // TODO: Maybe do this with a itterator and fixed length
    auto scenesToLoadLocal = m_ScenesToLoad;
    m_ScenesToLoad.clear();


    for(auto&& [id, loadMode] : scenesToLoadLocal)
    {
        if(not m_SceneBinds.contains(id))
            throw std::runtime_error(fmt::format("Scene {} has not been binded when loading", id));

        auto newSceneUPtr = std::make_unique<Scene>(id);

        // There should only ever be one override when loading here
        if(loadMode == SceneLoadMode::Override or loadMode == SceneLoadMode::OverrideForce)
            m_PrimaryScenePtr = newSceneUPtr.get();

        // Store scene
        m_LoadedScenes.emplace_back(std::move(newSceneUPtr));

        // Call load function
        m_SceneBinds[id](*m_LoadedScenes.back());
    }
}
