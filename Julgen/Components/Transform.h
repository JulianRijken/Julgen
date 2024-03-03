#pragma once
#include <unordered_set>
#include <glm/glm.hpp>

#include "Component.h"
//#include <glm/gtx/quaternion.hpp>

namespace jul
{
	class Transform final : public Component
	{

	public:
		Transform(glm::vec3 position = { 0,0,0 });
		~Transform() override;

		[[nodiscard]] const glm::vec3& WorldPosition();
		[[nodiscard]] const glm::vec3& LocalPosition() const { return m_LocalPosition; }

		[[nodiscard]] Transform* GetParent() const { return m_ParentPtr; }

		// There is no way to get a child by index because the children are stored in a set
		// Use GetChildren() and iterate through the set if you need to access children by index
		[[nodiscard]] int GetChildCount() const { return static_cast<int>(m_ChildPtrs.size()); }
		[[nodiscard]] const std::unordered_set<Transform*>& GetChildren() const { return m_ChildPtrs; }

		void SetLocalPosition(float x, float y, float z);
		void SetLocalPosition(const glm::vec3& position);

		//void SetLocalRotation(const glm::quat& rotation);

		//void SetLocalScale(float x, float y, float z);
		//void SetLocalScale(const glm::vec3& scale);

		void SetWorldPosition(float x, float y, float z);
		void SetWorldPosition(const glm::vec3& position);

		void Translate(float x, float y, float z);
		void Translate(const glm::vec3& translation);

		void SetParent(Transform* newParentPtr, bool worldPositionStays = true);

		bool IsChild(Transform* checkChildPtr) const;

	private:



		bool m_TransformDirty{true};



		glm::vec3 m_LocalPosition{};
		//glm::quat m_LocalRotation{};
		//glm::vec3 m_LocalScale{};


		glm::vec3 m_WorldPosition{};


		Transform* m_ParentPtr{nullptr};
		std::unordered_set<Transform*> m_ChildPtrs{};

		void UpdateWorldPosition();
		void SetTransformDirty();

		
	};
}
