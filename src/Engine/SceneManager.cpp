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

void jul::SceneManager::AddNewGameObjects()
{
    for(const auto& scene : m_LoadedScenes)
        scene->AddNewGameObjects();
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

    auto scenesToLoadLocal = m_ScenesToLoad;
    m_ScenesToLoad.clear();


    for(auto&& [id, loadMode] : scenesToLoadLocal)
    {
        if(not m_SceneBinds.contains(id))
            throw std::runtime_error(fmt::format("Scene {} has not been binded when loading", id));

        auto newSceneUPtr = std::make_unique<Scene>(id);

        // There should only ever be one override when loading here
        if(loadMode == SceneLoadMode::Override)
            m_PrimaryScenePtr = newSceneUPtr.get();

        // Store scene
        m_LoadedScenes.emplace_back(std::move(newSceneUPtr));

        // Call load function
        m_SceneBinds[id](*m_LoadedScenes.back());
    }

    // Wouw our user actually loaded a scene during the loading of the scene
    if(not m_ScenesToLoad.empty())
    {
        MarkScenesForUnload();
        CleanupGameObjects();
        CleanupScenes();
        LoadScenesSetToLoad();
    }
}
