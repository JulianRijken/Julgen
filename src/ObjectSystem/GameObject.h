#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <iostream>
#include <memory>
#include <vector>

#include "Object.h"
#include "Transform.h"

namespace jul
{
    class Scene;

    class GameObject final : public Object
    {
    public:
        GameObject(const std::string& name, Scene* scene, const glm::vec3& position = {});

        [[nodiscard]] Transform& GetTransform() const { return *m_TransformPtr; }

        [[nodiscard]] Scene& GetScene() const { return *m_ScenePtr; }

        [[nodiscard]] bool IsActiveInHierarchy();

        [[nodiscard]] bool IsActiveSelf() const { return m_ActiveSelf; }

        void SetActive(bool active);

        // Sets current game object to be destroyed
        // Including the components
        // But not the children
        void Destroy() override;

        // Sets all children to be destroyed
        void PropagateDestroy() const;

        void Update() const;
        void LateUpdate() const;
        void FixedUpdate() const;

        void CleanupComponents();

        // TODO: Make sure adding components during the update of the components is handled
        // (Components should avoid the update from the frame they are created in)
        template<typename ComponentType, typename... Args>
            requires std::derived_from<ComponentType, Component>
        ComponentType* AddComponent(Args&&... args)
        {
            // Avoid Transform from being added twice
            if constexpr(std::is_same_v<ComponentType, Transform>)
            {
                std::cerr << "Not allowed to add transform to game object\n";
                return static_cast<ComponentType*>(m_TransformPtr);
            }

            auto& addedComponent =
                m_Components.emplace_back(std::make_unique<ComponentType>(this, std::forward<Args>(args)...));

            return static_cast<ComponentType*>(addedComponent.get());
        }

        // Calls Destroy on the component
        // Ideally, Destroy() on the component is preferred
        void DestroyComponent(Component* component) { component->Destroy(); }

        template<typename ComponentType>
        // requires std::derived_from<ComponentType, Component> // TODO: Removed to allow Interfaces to work
        ComponentType* GetComponent() const
        {
            for(const auto& componentPtr : m_Components)
                if(auto castedComponentPtr = dynamic_cast<ComponentType*>(componentPtr.get()))
                    return castedComponentPtr;

            return nullptr;
        }

        // TODO: Add template specialization for getting the generic component
        //       This to avoid a dynamic cast
        template<typename ComponentType>
            requires std::derived_from<ComponentType, Component>
        std::vector<ComponentType*> GetComponents() const
        {
            std::vector<ComponentType*> components{};
            for(auto&& componentPtr : m_Components)
                if(auto* castedComponentPtr = dynamic_cast<ComponentType*>(componentPtr.get()))
                    components.push_back(castedComponentPtr);

            return components;
        }

        // Will also check on current game object
        template<typename ComponentType>
            requires std::derived_from<ComponentType, Component>
        ComponentType* GetComponentInParent() const
        {
            if(ComponentType* component = GetComponent<ComponentType>())
                return component;

            if(const Transform* parent = m_TransformPtr->GetParent())
                return parent->GetGameObject()->GetComponentInParent<ComponentType>();

            return nullptr;
        }

        // Will also check on current game object
        template<typename ComponentType>
            requires std::derived_from<ComponentType, Component>
        ComponentType* GetComponentInChildren() const
        {
            if(ComponentType* component = GetComponent<ComponentType>())
                return component;

            const auto& children = m_TransformPtr->GetChildren();
            for(auto&& child : children)
            {
                if(ComponentType* childComponent = child->GetGameObject()->GetComponentInChildren<ComponentType>())
                    return childComponent;
            }

            return nullptr;
        }

        template<typename ComponentType>
            requires std::derived_from<ComponentType, Component>
        [[nodiscard]] bool HasComponent() const
        {
            return GetComponent<ComponentType>() != nullptr;
        }

        void SetActiveDirty();

    private:
        void UpdateActiveInHierarchy();

        bool m_ActiveDirty{ true };
        bool m_ActiveSelf{ true };         // Primary Data
        bool m_ActiveInHierarchy{ true };  // Derived Data
        Scene* m_ScenePtr{ nullptr };
        Transform* m_TransformPtr{ nullptr };
        std::vector<std::unique_ptr<Component>> m_Components{};
    };
}  // namespace jul
#endif // GAMEOBJECT_H