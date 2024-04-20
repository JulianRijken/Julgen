#pragma once
#include <glm/vec3.hpp>
#include <memory>
#include <string>
#include <unordered_map>

#include "GameObject.h"
#include "Julgen.h"
#include "Scene.h"
#include "Singleton.h"


namespace jul
{

    enum class SceneLoadMode
    {
        Override,
        Additive,
    };

    class SceneManager final : public Singleton<SceneManager>
    {
        // Required to call update
        friend void Julgen::RunOneFrame();

    public:
        // TODO Loading scenes is currently done via functions
        //      this would ideally be done via file loading
        template<typename LoadSceneFunction>
        void LoadScene(const std::string& name, LoadSceneFunction loadScene,
                       SceneLoadMode loadMode = SceneLoadMode::Override)
        {
            if(loadMode == SceneLoadMode::Override)
            {
                // Unload all loaded scenes
                m_LoadedScenes.clear();
            }

            // Remove old scene if it has the same name
            if(m_LoadedScenes.contains(name))
                m_LoadedScenes.erase(name);

            // Set new loaded scene as active
            auto newSceneUPtr = std::make_unique<Scene>(name);

            if(loadMode == SceneLoadMode::Override)
                m_PrimaryScenePtr = newSceneUPtr.get();

            if constexpr(std::is_invocable_v<LoadSceneFunction, Scene&>)
                loadScene(*newSceneUPtr);
            else
                loadScene();

            m_LoadedScenes[name] = std::move(newSceneUPtr);
        }

        void UnloadScene(const std::string& sceneName);

        GameObject* AddGameObject(const std::string& name = "GameObject", const glm::vec3& position = {}) const;

    private:
        void Update();
        void LateUpdate();
        void FixedUpdate();

        void CleanupGameObjects();

        Scene* m_PrimaryScenePtr{};
        std::unordered_map<std::string, std::unique_ptr<Scene>> m_LoadedScenes{};
    };
}
