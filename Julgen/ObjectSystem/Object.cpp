#include "Object.h"

#include <iostream>


void Object::Destroy()
{
	m_BeingDestroyed = true;
	//std::cout << "X " << m_Name << '\n';
}

Object::Object(const std::string& name) :
	m_BeingDestroyed(false),
	m_Name(name)
{
	//std::cout << "+ " << m_Name << '\n';
}
