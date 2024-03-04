#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <glm/vec3.hpp>

#include "Singleton.h"

#include "Julgen.h"
#include "Scene.h"
#include "GameObject.h"


namespace jul
{

	class SceneManager final : public Singleton<SceneManager>
	{
		friend class Singleton;
		friend class Scene;

		// Required to call update
		friend void Julgen::RunOneFrame();


	public:

		void LoadScene(const std::string& name);

		GameObject* AddGameObject(const std::string& name = "GameObject", const glm::vec3& position = {}) const;


	private:

		void Update();
		void LateUpdate();
		void FixedUpdate();

		void Cleanup();

		SceneManager() = default;

	    Scene* m_ActiveScenePtr{};
		std::unordered_map<std::string, std::unique_ptr<Scene>> m_LoadedScenes{};
	};

}
