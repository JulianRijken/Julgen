#pragma once
#include <string>

class Object
{
public:

	[[nodiscard]] const std::string& GetName() const { return m_Name; }

protected:
	Object(const std::string& name = "Object");
	virtual ~Object() = default;

private:
	std::string m_Name;
};
