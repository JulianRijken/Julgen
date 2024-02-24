#include "Object.h"

#include <iostream>


void Object::Destroy()
{
	m_BeingDestroyed = true;
}

Object::Object(const std::string& name) :
	m_BeingDestroyed(false),
	m_Name(name)
{}
