#pragma once
#include <functional>
#include <glm/vec3.hpp>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <utility>

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
        friend class Julgen;

    public:
        // Binds the scene to a string allowing it to be loaded based on a name
        void BindScene(const std::string& name, std::function<void(Scene&)>&& sceneFunction)
        {
            m_SceneBinds[name] = std::move(sceneFunction);
        }

        // TODO Loading scenes is currently done via functions
        //      this would ideally be done via file loading
        void LoadScene(const std::string& name, SceneLoadMode loadMode = SceneLoadMode::Override)
        {
            if(loadMode == SceneLoadMode::Override)
                m_ScenesToLoad.clear();

            m_ScenesToLoad.emplace_back(name, loadMode);
        }

        GameObject* AddGameObject(const std::string& name = "GameObject", const glm::vec3& position = {}) const;


    private:
        void Update();
        void LateUpdate();
        void FixedUpdate();


        void Destroy();

        void CleanupGameObjects();
        void CleanupScenes();

        void MarkScenesForUnload();
        void LoadScenesSetToLoad();

        Scene* m_PrimaryScenePtr{};

        // These scenes will be loaded at the end of the frame
        std::vector<std::pair<std::string, SceneLoadMode>> m_ScenesToLoad{};

        std::vector<std::unique_ptr<Scene>> m_LoadedScenes{};
        std::unordered_map<std::string, std::function<void(Scene&)>> m_SceneBinds{};
    };
}
