#ifndef SCENE_H
#define SCENE_H
#include <glm/vec3.hpp>
#include <memory>
#include <string>
#include <vector>

#include "GameObject.h"


namespace jul
{
	class Scene final
	{
        friend class SceneManager;

    public:
        explicit Scene(int id);
        ~Scene() = default;

        GameObject* AddGameObject(const std::string& name, const glm::vec3& position = {},
                                  const GameObject* parent = nullptr, bool worldPositionStays = true);

        [[nodiscard]] int GetId() const { return m_Id; }

        // Unloads whole scene
        void Unload();


    private:
        void Update() const;
        void LateUpdate() const;
		void FixedUpdate() const;

        void MoveGameObjectsAdded();
        void MarkGameObjectsForDestroy();
        void CleanupGameObjects();

        int m_Id;
        bool m_BeingUnloaded{ false };

        std::vector<std::unique_ptr<GameObject>> m_GameObjectsAdded{};
        std::vector<std::unique_ptr<GameObject>> m_GameObjectsInScene{};
    };

}  // namespace jul
#endif // SCENE_H