#pragma once
#include "Singleton.h"

namespace jul
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
	};
}
