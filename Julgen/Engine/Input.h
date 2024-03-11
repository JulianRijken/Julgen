#pragma once
#include <unordered_set>

#include "Singleton.h"
#include "Command.h"

namespace jul
{
	class Input final : public Singleton<Input>
	{
	public:
		bool ProcessInput();


		template <typename CommandType, typename... Args>
			requires std::derived_from<CommandType, BaseCommand>
		void RegisterCommand(Args&&... args)
		{
			m_Commands.emplace(std::make_unique<CommandType>(args...));
		}


	private:
		std::unordered_set<std::unique_ptr<BaseCommand>> m_Commands;
	};
}
