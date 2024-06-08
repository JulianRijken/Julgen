#include "Object.h"

#include <Julgen.h>

#include <cassert>

#include "TweenEngine.h"

void jul::Object::Destroy()
{
    // We want to ensure objects are destoryed in the right order
    assert((Julgen::g_LoopState != Julgen::LoopState::MarkScenesForUnload and
            Julgen::g_LoopState != Julgen::LoopState::CleanupGameObjects and
            Julgen::g_LoopState != Julgen::LoopState::CleanupScenes and
            Julgen::g_LoopState != Julgen::LoopState::LoadScenesSetToLoad) &&
           "Object Destroyed Before Destroy Called");

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
    assert(Julgen::g_LoopState == Julgen::LoopState::CleanupGameObjects);

    // We want to ensure objects are destoryed in the right order
    if(not m_BeingDestroyed)
        assert(false && "Object Destroyed Before Destroy Called");
}
