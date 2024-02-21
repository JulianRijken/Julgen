#pragma once

#include <memory>
#include <vector>

#include "Component.h"
#include "Object.h"


namespace jul
{
	class Transform;

	class GameObject final : public Object
	{
		friend class SceneManager;
		friend class Scene;

	public:

		[[nodiscard]] Transform& GetTransform() const { return *m_TransformPtr; }

		template<class ComponentType, class... Args>
		std::enable_if_t<std::is_base_of_v<Component, ComponentType>, ComponentType&>
			AddComponent(Args&&... args)
		{
			std::shared_ptr<ComponentType> addedComponent{ std::make_shared<ComponentType>(std::forward<Args>(args)...) };
			m_Components.emplace_back(addedComponent);

			Component* castComponentPtr{ static_cast<Component*>(addedComponent.get()) };
			castComponentPtr->m_ParentGameObjectPtr = this;

			return *addedComponent;
		}

	private:

		void Update();
		void Render() const;

		Transform* m_TransformPtr;
		std::vector<std::shared_ptr<Component>> m_Components{};

		GameObject(const std::string& name);
	};
}
