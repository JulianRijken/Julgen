#pragma once
#include <string>
#include <memory>
#include <unordered_map>

#include "Singleton.h"
#include "glm/vec3.hpp"


namespace jul
{
	class TextRenderer;
	class Scene;
	class GameObject;

	class SceneManager final : public Singleton<SceneManager>
	{
		friend class Singleton;

		// Required to call update and render
		friend class Julgen;
		friend class RenderManager;

	public:

		void LoadScene(const std::string& name);

		GameObject* AddGameObject(const std::string& name = "GameObject", const glm::vec3& position = {}) const;


	private:

		void Update();
		void LateUpdate();
		void FixedUpdate();

		void Cleanup();

		SceneManager() = default;

		std::shared_ptr<Scene> m_ActiveScene{};

		// TODO: Scenes are now stored with their name, but this might be redundant as the scene class also stores the name
		std::unordered_map<std::string, std::shared_ptr<Scene>> m_LoadedScenes{};
	};

}
