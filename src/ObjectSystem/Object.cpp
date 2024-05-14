#include "Object.h"

void jul::Object::Destroy() { m_BeingDestroyed = true; }

jul::Object::Object(std::string name) :
    m_Name(std::move(name))
{}
