#pragma once
#include <unordered_set>
#include <glm/glm.hpp>


namespace jul
{
	class Transform final
	{
	public:
		Transform(glm::vec3 position = {0,0,0});

		[[nodiscard]] const glm::vec3& WorldPosition();
		[[nodiscard]] const glm::vec3& LocalPosition() const { return m_LocalPosition; }

		[[nodiscard]] Transform* GetParent() const { return m_ParentPtr; }
		[[nodiscard]] int GetChildCount() const { return static_cast<int>(m_ChildPtrs.size()); }
		[[nodiscard]] const std::unordered_set<Transform*>& GetChildren() const { return m_ChildPtrs; }

		void SetLocalPosition(float x, float y, float z);
		void SetLocalPosition(const glm::vec3& position);

		void SetWorldPosition(float x, float y, float z);
		void SetWorldPosition(const glm::vec3& position);

		void Translate(float x, float y, float z);
		void Translate(const glm::vec3& translation);

		void SetParent(Transform* newParentPtr, bool worldPositionStays = true);

		bool IsChild(Transform* checkChildPtr) const;

	private:

		bool m_PositionDirty{true};

		glm::vec3 m_LocalPosition{};
		glm::vec3 m_WorldPosition{};

		Transform* m_ParentPtr{nullptr};
		std::unordered_set<Transform*> m_ChildPtrs{};

		void UpdateWorldPosition();
		void SetPositionDirty();
	};
}
