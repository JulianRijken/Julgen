#include "TweenInstance.h"

#include "GameTime.h"
#include "Tween.h"

jul::TweenInstance::TweenInstance(Tween&& tween, GameObject* target) :
    m_Tween(std::move(tween)),
    m_Target(target)
{
}

void jul::TweenInstance::Update()
{
    if(m_Tween.onStart)
    {
        m_Tween.onStart();
        m_Tween.onStart = {};  // Remove start function as soon as called
    }

    const double deltaTime = m_Tween.igunoreTimeScale ? GameTime::GetUnScaledDeltaTime() : GameTime::GetDeltaTime();
    m_Time += deltaTime;

    if(m_Time >= m_Tween.duration)
    {
        m_Time = m_Tween.duration;
        m_HasReachedEnd = true;
    }


    // TODO: Add ease function in to the mix (linear now)
    const double alpha = m_Time / m_Tween.duration;
    const double easedTime = EaseFunction::Evaluate(alpha, m_Tween.easeFunction);
    const double interpolatedValue = std::lerp(m_Tween.from, m_Tween.to, easedTime);


    // TODO: Should never not be set, but might not be needed
    // in the future when adding specific tweens like a positon tween
    if(m_Tween.onUpdate)
        m_Tween.onUpdate(interpolatedValue);

    if(m_HasReachedEnd)
    {
        if(m_Tween.onEnd)
            m_Tween.onEnd();

        m_IsDecommissioned = true;
    }
}
