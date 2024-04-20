#pragma once
#include <list>
#include <memory>
#include <string>

namespace jul
{
	class GameObject;

	class Scene final
	{
		friend class SceneManager;

	public:
		Scene(const std::string& name);
		~Scene() = default;

	private:

		void Update() const;
		void LateUpdate() const;
		void FixedUpdate() const;

		void CleanupGameObjects();

		GameObject* AddGameObjectToScene(std::unique_ptr<GameObject>&& object);


		std::string m_Name;
		std::list<std::unique_ptr<GameObject>> m_GameObjectsInSceneSPtr{};
	};

}
