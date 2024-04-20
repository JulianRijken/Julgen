#pragma once
#include <glm/vec3.hpp>
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

        GameObject* AddGameObject(const std::string& name, const glm::vec3& position);

    private:
        void Update() const;
        void LateUpdate() const;
		void FixedUpdate() const;

		void CleanupGameObjects();


        std::string m_Name;
        std::list<std::unique_ptr<GameObject>> m_GameObjectsInSceneSPtr{};
    };

}  // namespace jul
