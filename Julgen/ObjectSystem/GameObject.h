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
		requires std::derived_from<ComponentType, Component>
		ComponentType& AddComponent(Args&&... args)
		{
			auto addedComponent = std::make_shared<ComponentType>(std::forward<Args>(args)...);
			m_Components.push_back(addedComponent);

			const auto castComponentPtr{static_cast<Component*>(addedComponent.get())};
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
