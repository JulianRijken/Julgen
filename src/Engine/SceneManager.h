#pragma once
#include <functional>
#include <glm/vec3.hpp>
#include <memory>
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
        [[nodiscard]] Scene& GetPrimaryScene() const { return *m_PrimaryScenePtr; }

        // Binds the scene to a string allowing it to be loaded based on a name
        void BindScene(int id, std::function<void(Scene&)>&& sceneFunction);

        void LoadScene(int id, SceneLoadMode loadMode = SceneLoadMode::Override);

        [[nodiscard]] Scene* FindScene(int id);

        template<typename... Args>
        GameObject* AddGameObject(Args... args) const
        {
            return m_PrimaryScenePtr->AddGameObject(std::forward<Args>(args)...);
        }


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
        std::vector<std::pair<int, SceneLoadMode>> m_ScenesToLoad{};

        std::vector<std::unique_ptr<Scene>> m_LoadedScenes{};
        std::unordered_map<int, std::function<void(Scene&)>> m_SceneBinds{};
    };
}
