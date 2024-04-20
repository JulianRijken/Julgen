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

    class SceneManager final : public Singleton<SceneManager>
    {
        friend class Scene;

		// Required to call update
		friend void Julgen::RunOneFrame();


	public:
        // TODO Loading scenes is currently done via functions
        //      this would ideally be done via file loading
        template<typename LoadSceneFunction>
        void LoadScene(const std::string& name, LoadSceneFunction loadScene)
        {
            // Unload all loaded scenes
            m_LoadedScenes.clear();

            // Set new loaded scene as active
            auto newSceneUPtr = std::make_unique<Scene>(name);
            m_ActiveScenePtr = newSceneUPtr.get();
            m_LoadedScenes[name] = std::move(newSceneUPtr);

            loadScene();

            // TODO: Game objects and components can still be created without adding them to a scene
            //        or having them attached to a game object
            //        This is still possible tho. Look in to this :)
            //        Maybne use camke and private link
            //      [[maybe_unused]] GameObject* wrongGameObject = new GameObject("Test", glm::vec3(1, 1, 1));
            //      [[maybe_unused]] FpsCounter* wrongFpsCounter = new FpsCounter(wrongGameObject);
        }

        GameObject* AddGameObject(const std::string& name = "GameObject", const glm::vec3& position = {}) const;

    private:
        void Update();
        void LateUpdate();
        void FixedUpdate();

        void CleanupGameObjects();

        Scene* m_ActiveScenePtr{};
        std::unordered_map<std::string, std::unique_ptr<Scene>> m_LoadedScenes{};
    };
}
