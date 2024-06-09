#include "TweenInstance.h"

#include "GameTime.h"
#include "Object.h"
#include "Tween.h"

jul::TweenInstance::TweenInstance(Tween&& tween, Object* target) :
    m_IsHalting(tween.delay > 0),
    m_Target(target),
    m_Tween(std::move(tween))
{
    // Sets target to nullptr when detroyed
    m_Target->GetOnDestroyedEvent().AddListener(this, &TweenInstance::OnTargetDestroyed);
}

void jul::TweenInstance::OnTargetDestroyed(Object*) { m_Target = nullptr; }

void jul::TweenInstance::Cancel()
{
    if(m_Tween.invokeWhenDestroyed and m_Target == nullptr)
    {
        if(m_Tween.onEnd)
            m_Tween.onEnd();
    }

    m_IsDecommissioned = true;
}


void jul::TweenInstance::Update()
{
    if(m_Target == nullptr)
    {
        Cancel();
        return;
    }

    const double deltaTime = m_Tween.igunoreTimeScale ? GameTime::GetUnScaledDeltaTime() : GameTime::GetDeltaTime();

    if(m_IsHalting)
    {
        m_Tween.delay -= deltaTime;
        if(m_Tween.delay > 0)
            return;

        // Compensate for overshot;
        m_Tween.duration += m_Tween.delay;

        m_IsHalting = false;
    }

    if(m_Tween.onStart)
    {
        m_Tween.onStart();
        m_Tween.onStart = {};  // Remove start function as soon as called
    }

    m_Time += deltaTime;

    // Yes, deltatime can go below 0
    // when we change time scale :)
    m_Time = std::max(0.0, m_Time);

    if(m_Time >= m_Tween.duration)
    {
        m_Time = m_Tween.duration;
        m_HasReachedEnd = true;
    }


    const double alpha = m_Time / m_Tween.duration;
    const double easedTime = EaseFunction::Evaluate(alpha, m_Tween.easeFunction);
    const double interpolatedValue = std::lerp(m_Tween.from, m_Tween.to, easedTime);


    if(m_Tween.onUpdate)
        m_Tween.onUpdate(interpolatedValue);

    if(m_HasReachedEnd)
    {
        if(m_Tween.onEnd)
            m_Tween.onEnd();

        m_IsDecommissioned = true;
    }
}
