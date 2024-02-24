#pragma once
#include "Component.h"

namespace jul
{
	// TODO: Currently behaviour does nothing and unless be made usefull should just be removed
	class Behaviour : public Component
	{
		friend class GameObject;

	public:

		~Behaviour() override = default;

		Behaviour(Behaviour&&) = delete;
		Behaviour(const Behaviour&) = delete;
		Behaviour& operator=(Behaviour&&) = delete;
		Behaviour& operator=(const Behaviour&) = delete;

	protected:

		Behaviour(const std::string& name = "Behaviour");
	};

}
