#pragma once
#include <iostream>
#include <list>
#include <memory>

#include "Object.h"
#include "Transform.h"

namespace jul
{
    class GameObject final : public Object
    {
    public:
        GameObject(const std::string& name, const glm::vec3& position);

        [[nodiscard]] Transform& GetTransform() const { return *m_TransformPtr; }

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

        template<typename ComponentType, typename... Args>
            requires std::derived_from<ComponentType, Component>
        ComponentType* AddComponent(Args&&... args)
        {
            // Avoid Transform from being added twice
            if constexpr(std::is_same_v<ComponentType, Transform>)
            {
                std::cerr << "Not allowed to add transform to game object\n";
                return reinterpret_cast<ComponentType*>(m_TransformPtr);
            }

            auto& addedComponent =
                m_Components.emplace_back(std::make_unique<ComponentType>(this, std::forward<Args>(args)...));

            // TODO: Awake is currently redundant and should be only called at the end of the frame to allow
            // components to find each other
            addedComponent->Awake();

            return reinterpret_cast<ComponentType*>(addedComponent.get());
        }

        // Calls Destroy on the component
        // Ideally, Destroy() on the component is preferred
        void DestroyComponent(Component* component) { component->Destroy(); }

        template<typename ComponentType>
            requires std::derived_from<ComponentType, Component>
        ComponentType* GetComponent() const
        {
            for(const auto& componentPtr : m_Components)
                if(auto castedComponentPtr = dynamic_cast<ComponentType*>(componentPtr.get()))
                    return castedComponentPtr;

            return nullptr;
        }

        // Will also check on current game object
        template<typename ComponentType>
            requires std::derived_from<ComponentType, Component>
        ComponentType* GetComponentOnParent() const
        {
            ComponentType* component = GetComponent<ComponentType>();
            if(component)
                return component;

            Transform* parent = m_TransformPtr->GetParent();

            if(parent)
                return parent->GetGameObject()->GetComponentOnParent<ComponentType>();

            return nullptr;
        }

        template<typename ComponentType>
            requires std::derived_from<ComponentType, Component>
        [[nodiscard]] bool HasComponent() const
        {
            return GetComponent<ComponentType>() != nullptr;
        }

    private:
        Transform* m_TransformPtr;
        std::list<std::unique_ptr<Component>> m_Components{};
    };
}  // namespace jul
