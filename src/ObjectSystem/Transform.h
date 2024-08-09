#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
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
        [[nodiscard]] const glm::quat& GetWorldRotation();
        [[nodiscard]] const glm::vec3& GetWorldScale();
        [[nodiscard]] const glm::mat4& GetWorldMatrix();
        [[nodiscard]] glm::vec3 GetEulerAngles();

        [[nodiscard]] const glm::vec3& GetLocalPosition() const { return m_LocalPosition; }

        [[nodiscard]] const glm::quat& GetLocalRotation() const { return m_LocalRotation; }

        [[nodiscard]] const glm::vec3& GetLocalScale() const { return m_LocalScale; }

        [[nodiscard]] Transform* GetParent() const { return m_ParentPtr; }

        // There is no way to get a child by index because the children are stored in a set
        // Use GetChildren() and iterate through the set if you need to access children by index
        [[nodiscard]] int GetChildCount() const { return static_cast<int>(m_ChildPtrs.size()); }
        [[nodiscard]] const std::unordered_set<Transform*>& GetChildren() const { return m_ChildPtrs; }

        void SetLocalPosition(double x, double y, double z);
        void SetLocalPosition(const glm::vec3& position);
        void SetLocalRotation(double x, double y, double z);
        void SetLocalRotation(const glm::vec3& rotation);
        void SetLocalRotation(const glm::quat& rotation);
        void SetLocalScale(double x, double y, double z);
        void SetLocalScale(const glm::vec3& scale);

        void SetWorldPosition(double x, double y, double z);
        void SetWorldPosition(const glm::vec3& position);
        void SetWorldRotation(double x, double y, double z);
        void SetWorldRotation(const glm::vec3& rotation);
        void SetWorldRotation(const glm::quat& rotation);
        void SetWorldScale(double x, double y, double z);
        void SetWorldScale(const glm::vec3& scale);

        void Translate(double x, double y, double z);
        void Translate(const glm::vec3& translation);

        void Rotate(double x, double y, double z);
        void Rotate(const glm::vec3& eulerAngles);


        void SetParent(Transform* newParentPtr, bool worldPositionStays = true);

        bool IsChild(Transform* checkChildPtr) const;

        // Used by transform to force update physics position
        void SetRigidbody(Rigidbody* rigidbody);

    private:
        void UpdateWorldPosition();
        void UpdateWorldRotation();
        void UpdateWorldScale();
        void UpdateWorldMatrix();


        void SetPositionDirty();
        void SetRotationDirty();
        void SetScaleDirty();

        void OnRigidbodyDestroyed(Object* object);

        glm::vec3 m_LocalPosition{ 0, 0, 0 };            // Primary Data
        glm::quat m_LocalRotation{ glm::mat4{ 1.0f } };  // Primary Data
        glm::vec3 m_LocalScale{ 1, 1, 1 };               // Primary Data


        bool m_PositionDirty{ true };
        bool m_RotationDirty{ true };
        bool m_ScaleDirty{ true };
        bool m_MatrixDirty{ true };

        glm::vec3 m_WorldPosition{};  // Derived Data
        glm::quat m_WorldRotation{};  // Derived Data
        glm::vec3 m_WorldScale{};     // Derived Data

        glm::mat4 m_WorldMatrix{};

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
