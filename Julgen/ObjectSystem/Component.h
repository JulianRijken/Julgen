#pragma once

#include "Object.h"


namespace jul
{
	class Transform;

	class Component : public Object
	{
		friend class GameObject;

	public:

		~Component() override = default;

		Component(Component&&) = delete;
		Component(const Component&) = delete;
		Component& operator=(Component&&) = delete;
		Component& operator=(const Component&) = delete;

		[[nodiscard]] GameObject* GetGameObject() const { return m_ParentGameObjectPtr;  }
		[[nodiscard]] Transform& Transform() const;

	protected:

		Component(const std::string& name = "Component");

		virtual void Update() {}
		virtual void LateUpdate() {}
		virtual void FixedUpdate() {}

		virtual void Awake() {}

	private:

		GameObject* m_ParentGameObjectPtr{};
	};
}

