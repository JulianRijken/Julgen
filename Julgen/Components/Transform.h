#pragma once
#include <glm/glm.hpp>


namespace jul
{
	class Transform final
	{
	public:
		Transform(glm::vec3 position = {0,0,0});

		[[nodiscard]] const glm::vec3& Position() const { return m_Position; }


		void SetPosition(float x, float y, float z);
		void SetPosition(const glm::vec3& position);

		void Translate(float x, float y, float z);
		void Translate(const glm::vec3& translation);

	private:

		glm::vec3 m_Position{};
	};
}
