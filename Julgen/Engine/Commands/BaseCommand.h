#pragma once

class BaseCommand
{
public:
	virtual ~BaseCommand() = default;
	virtual void Execute() = 0;
};
