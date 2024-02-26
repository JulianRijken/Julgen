#pragma once

#include <list>
#include <memory>

#include "Component.h"
#include "Object.h"
#include "Transform.h"

namespace jul
{

	class GameObject final : public Object
	{
		friend class SceneManager;
		friend class Scene;

	public:

		[[nodiscard]] Transform& GetTransform() { return m_Transform; }


		void Destroy() override;


		template <typename ComponentType, typename... Args>
		requires std::derived_from<ComponentType, Component>
		ComponentType* AddComponent(Args&&... args) 		
		{
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

		void Cleanup();

		void Update() const;
		void LateUpdate() const;
		void FixedUpdate() const;

		Transform m_Transform;
		std::list<std::unique_ptr<Component>> m_Components{};

		GameObject(const std::string& name, const glm::vec3& position);
	};
}
