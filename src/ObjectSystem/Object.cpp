#include "Object.h"

void jul::Object::Destroy() { m_BeingDestroyed = true; }

jul::Object::Object(const std::string& name) :
    m_BeingDestroyed(false),
    m_Name(name)
{
}
