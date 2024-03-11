#pragma once
#include "BaseCommand.h"

class JumpCommand : public BaseCommand
{
public:
	virtual ~JumpCommand() = default;
	void Execute() override = { Jump(); }
};
