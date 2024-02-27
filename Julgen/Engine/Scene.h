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

		void Update() const;
		void LateUpdate() const;
		void FixedUpdate() const;

		void Cleanup();

		GameObject* AddGameObjectToScene(std::unique_ptr<GameObject>&& object);

		explicit Scene(const std::string& name);

		std::string m_Name;
		std::list<std::unique_ptr<GameObject>> m_GameObjectsInSceneSPtr{};
	};

}
