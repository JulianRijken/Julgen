#pragma once
#include "Component.h"

#include <glm/glm.hpp>


namespace jul
{
	class Transform final : public Component
	{

	public:
		Transform(glm::vec3 position = {0,0,0});

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(float x, float y, float z);

	private:


		glm::vec3 m_Position{};
	};
}
