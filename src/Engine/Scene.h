#pragma once
#include <glm/vec3.hpp>
#include <memory>
#include <string>
#include <vector>


namespace jul
{
	class GameObject;

	class Scene final
	{
        friend class SceneManager;

    public:
        explicit Scene(std::string name);
        ~Scene() = default;

        GameObject* AddGameObject(const std::string& name, const glm::vec3& position = {},
                                  const GameObject* parent = nullptr);

    private:
        void Update() const;
        void LateUpdate() const;
		void FixedUpdate() const;

        void CleanupGameObjects();

        void MarkForUnload();


        std::string m_Name;
        bool m_BeingUnloaded{ false };
        std::vector<std::unique_ptr<GameObject>> m_GameObjectsInSceneSPtr{};
    };

}  // namespace jul
