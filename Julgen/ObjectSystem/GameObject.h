#pragma once
#include "Object.h"

#include <list>
#include <memory>

#include "SceneManager.h"
#include "Transform.h"
#include "Component.h"


namespace jul
{

	class GameObject final : public Object
	{
		friend GameObject* SceneManager::AddGameObject(const std::string& name, const glm::vec3& position) const;

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

			if(IsBeingDestroyed())
				addedComponent->Destroy();

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

		void Cleanup();

		void Update() const;
		void LateUpdate() const;
		void FixedUpdate() const;

		Transform m_Transform;
		std::list<std::unique_ptr<Component>> m_Components{};

	};
}
