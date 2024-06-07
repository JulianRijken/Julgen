#include "Object.h"

#include <cassert>

#include "TweenEngine.h"

void jul::Object::Destroy()
{
    if(not m_BeingDestroyed)
    {
        // Importaint that m_BeingDestroyed is set to true
        // BEFORE the event, this is so that weens and other objects
        // can check for being destroyed at event time
        m_BeingDestroyed = true;
        m_OnDestroyedEvent.Invoke();
    }
    else
    {
        m_BeingDestroyed = true;
    }
}

void jul::Object::DestroyDelayed(double delay)
{
    TweenEngine::Start({ .delay = delay,
                         .duration = 0,
                         .invokeWhenDestroyed = false,
                         // Destroy Delayed
                         .onEnd = [this]() { Destroy(); } },
                       this);
}


jul::Object::Object(std::string name) :
    m_Name(std::move(name))
{
}

jul::Object::~Object()
{
    // We want to ensure objects are destoryed in the right order
    if(not m_BeingDestroyed)
        assert(false && "Object Destroyed Before Destroy Called");
}
