#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <glm/glm.hpp>
#include <unordered_set>

#include "Component.h"
#include "Rigidbody.h"

namespace jul
{
    class Transform final : public Component, public IEventListener
    {

    public:
        Transform(GameObject* parentPtr, const glm::vec3& position = { 0, 0, 0 });
        ~Transform() override;

        Transform(const Transform&) = delete;
        Transform(Transform&&) noexcept = delete;
        Transform& operator=(const Transform&) = delete;
        Transform& operator=(Transform&&) noexcept = delete;

        [[nodiscard]] const glm::vec3& GetWorldPosition();

        [[nodiscard]] const glm::vec3& GetLocalPosition() const { return m_LocalPosition; }

        [[nodiscard]] Transform* GetParent() const { return m_ParentPtr; }

        // There is no way to get a child by index because the children are stored in a set
        // Use GetChildren() and iterate through the set if you need to access children by index
        [[nodiscard]] int GetChildCount() const { return static_cast<int>(m_ChildPtrs.size()); }
        [[nodiscard]] const std::unordered_set<Transform*>& GetChildren() const { return m_ChildPtrs; }

        void SetLocalPosition(double x, double y, double z);
        void SetLocalPosition(const glm::vec3& position);

        void SetWorldPosition(double x, double y, double z);
        void SetWorldPosition(const glm::vec3& position);

        void Translate(double x, double y, double z);
        void Translate(const glm::vec3& translation);

		void SetParent(Transform* newParentPtr, bool worldPositionStays = true);

		bool IsChild(Transform* checkChildPtr) const;

        // Used by transform to force update physics position
        void SetRigidbody(Rigidbody* rigidbody);

    private:
        void UpdateWorldPosition();
        void SetTransformDirty();
        void OnRigidbodyDestroyed(Object* object);

        bool m_TransformDirty{ true };
        glm::vec3 m_LocalPosition{};  // Primary Data
        glm::vec3 m_WorldPosition{};  // Derived Data

        // Transform needs acces to the optional rigidbody
        // this is because it needs to update the physics positions
        // when setting the position manually
        Rigidbody* m_Rigidbody{ nullptr };

        Transform* m_ParentPtr{ nullptr };

        // TODO: Perform performance tests on the use of unordered_set instead of vector
        std::unordered_set<Transform*> m_ChildPtrs{};
    };
}
#endif // TRANSFORM_H