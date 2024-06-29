#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include <functional>
#include <glm/vec3.hpp>
#include <memory>
#include <unordered_map>
#include <utility>

#include "GameObject.h"
#include "Julgen.h"
#include "Scene.h"
#include "Singleton.h"

namespace jul
{
    // Override: Clears all other scenes set to load
    // OverrideForce: Force loads scene current frame and negates otehr overrides that come after during that frame
    // Additive: Adds to current scenes set to load
    enum class SceneLoadMode
    {
        Override,
        OverrideForce,
        Additive,
    };

    class SceneManager final : public Singleton<SceneManager>
    {
        friend class Julgen;

    public:
        [[nodiscard]] static Scene& GetPrimaryScene() { return *GetInstance().m_PrimaryScenePtr; }

        [[nodiscard]] Scene* FindScene(int id);

        // Binds the scene to a string allowing it to be loaded based on a name
        void BindScene(int id, std::function<void(Scene&)>&& sceneFunction);

        void LoadScene(int id, SceneLoadMode loadMode = SceneLoadMode::OverrideForce);


    private:
        void Update();
        void LateUpdate();
        void FixedUpdate();

        void Destroy();

        void ProcessScenes();

        void MarkScenesForUnload();
        void MarkGameObjectsForDestroy();
        void CleanupGameObjects();
        void CleanupScenes();
        void LoadScenesSetToLoad();

        Scene* m_PrimaryScenePtr{};

        // These scenes will be loaded at the end of the frame
        std::vector<std::pair<int, SceneLoadMode>> m_ScenesToLoad{};

        std::vector<std::unique_ptr<Scene>> m_LoadedScenes{};
        std::unordered_map<int, std::function<void(Scene&)>> m_SceneBinds{};
    };
}
#endif // SCENEMANAGER_H