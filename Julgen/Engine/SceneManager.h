#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"


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



		// Loads scene by name
		void LoadScene(const std::string& name);

		// Adds a new game object to the active scene
		GameObject& AddGameObject(const std::string& name);



		/// Optional for future
		//// Creates empty scene at run time
		//void CreateScene(const std::string& name);

		//MoveGameObjectToScene() 


	private:


		void Update();
		void LateUpdate();
		void FixedUpdate();
		void Render() const;

		SceneManager() = default;

		std::shared_ptr<Scene> m_ActiveScene{};
		//std::vector<std::shared_ptr<Scene>> m_Scenes{};

		TextRenderer* REMOVEME;
	};

}
