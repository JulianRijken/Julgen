#pragma once

#include "Object.h"

namespace jul
{

    class GameObject;
    class Transform;

    class Component : public Object
    {
    public:
        explicit Component(GameObject* parentPtr = nullptr, const std::string& name = "Component");
        ~Component() override = default;

        Component(const Component&) = delete;
        Component(Component&&) noexcept = delete;
        Component& operator=(const Component&) = delete;
        Component& operator=(Component&&) noexcept = delete;

        [[nodiscard]] GameObject* GetGameObject() const { return m_ParentGameObjectPtr; }

        [[nodiscard]] Transform& GetTransform() const;

        [[nodiscard]] bool IsEnabled() const { return m_IsEnabled; }

        [[nodiscard]] bool IsEnabledAndActive() const;

        void SetEnabled(bool enabled);

        virtual void Update() {}
        virtual void LateUpdate() {}
		virtual void FixedUpdate() {}


    private:
        GameObject* m_ParentGameObjectPtr{};
        bool m_IsEnabled{ true };
    };
}

