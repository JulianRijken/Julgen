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

        [[nodiscard]] inline GameObject* GetGameObject() const { return m_ParentGameObjectPtr; }

        [[nodiscard]] Transform& GetTransform() const;

        virtual void Update() {}
        virtual void LateUpdate() {}
		virtual void FixedUpdate() {}


    private:
        GameObject* m_ParentGameObjectPtr{};
    };
}

