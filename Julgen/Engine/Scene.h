#pragma once
#include "SceneManager.h"

namespace jul
{
	class GameObject;

	class Scene final
	{
		friend class SceneManager;

		void Update();
		void Render() const;

		void AddGameObjectToScene(std::shared_ptr<GameObject> object);

		explicit Scene(const std::string& name);

		std::string m_Name;
		std::vector<std::shared_ptr<GameObject>> m_GameObjectsInSceneSPtr{};
	};

}
