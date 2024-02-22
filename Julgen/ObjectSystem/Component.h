#pragma once

#include "Object.h"


namespace jul
{
	class Transform;
	class GameObject;

	class Component : public Object
	{
		friend class GameObject;

	public:

		Component(const std::string& name = "Component");
		~Component() override = default;

		Component(Component&&) = delete;
		Component(const Component&) = delete;
		Component& operator=(Component&&) = delete;
		Component& operator=(const Component&) = delete;

		[[nodiscard]] GameObject& GetGameObject() const { return *m_ParentGameObjectPtr;  }
		[[nodiscard]] Transform& GetTransform() const;

	private:

		GameObject* m_ParentGameObjectPtr;
	};
}

