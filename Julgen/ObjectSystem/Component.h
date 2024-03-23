#pragma once
#include "Object.h"


namespace jul
{
	class GameObject;
	class Transform;

	class Component : public Object
	{

	public:
		explicit Component(GameObject* parent = nullptr, const std::string& name = "Component");
        ~Component() override = default;

        Component(const Component&) = delete;
        Component(Component&&) noexcept = delete;
        Component& operator=(const Component&) = delete;
        Component& operator=(Component&&) noexcept = delete;

		[[nodiscard]] GameObject* GetGameObject() const { return m_ParentGameObjectPtr;  }
		[[nodiscard]] Transform& Transform() const;

		virtual void Update() {}
		virtual void LateUpdate() {}
		virtual void FixedUpdate() {}

		virtual void Awake() {}

	private:

		GameObject* m_ParentGameObjectPtr{};
	};
}

