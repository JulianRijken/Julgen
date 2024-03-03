#pragma once
#include <iostream>

#include "Object.h"

#include <list>
#include <memory>

#include "Transform.h"
#include "Component.h"


namespace jul
{

	class GameObject final : public Object
	{
		// TODO: Using friend class instead of friend function to avoid include of SceneManager.h, is this better than the include?
		friend class Scene;
		friend class SceneManager;

	public:

		void Destroy() override;

		[[nodiscard]] Transform& GetTransform() const { return *m_TransformPtr; }

		template <typename ComponentType, typename... Args>
		requires std::derived_from<ComponentType, Component>
		ComponentType* AddComponent(Args&&... args) 		
		{
			// Avoid Transform from being added twice
			if constexpr (std::is_same_v<ComponentType, Transform>)
			{
				std::cerr << "Not allowed to add transform to game object\n";
				return reinterpret_cast<ComponentType*>(m_TransformPtr);
			}

			auto& addedComponent = m_Components.emplace_back(std::make_unique<ComponentType>(args...));
			addedComponent->m_ParentGameObjectPtr = this;
			addedComponent->Awake();

			return reinterpret_cast<ComponentType*>(addedComponent.get());
		}

		// Calls Destroy on the component
		// Ideally, Destroy() on the component is preferred
		//template <typename ComponentType>
		//requires std::derived_from<ComponentType, Component>
		void DestroyComponent(Component* component)
		{
			component->Destroy();
		}


		template <typename ComponentType>
		requires std::derived_from<ComponentType, Component>
		ComponentType* GetComponent() const
		{
			for (const auto& componentPtr : m_Components)
			{
				if (auto castedComponentPtr = dynamic_cast<ComponentType*>(componentPtr.get()))
					return castedComponentPtr;
			}

			return nullptr;
		}


		template <typename ComponentType>
		requires std::derived_from<ComponentType, Component>
		bool HasComponent() const
		{
			return GetComponent<ComponentType>() != nullptr;
		}

	private:

		GameObject(const std::string& name, const glm::vec3& position);

		void PropagateDestroy() const;
		void Cleanup();

		void Update() const;
		void LateUpdate() const;
		void FixedUpdate() const;

	private:
		Transform* m_TransformPtr;
		std::list<std::unique_ptr<Component>> m_Components{};
	};
}
