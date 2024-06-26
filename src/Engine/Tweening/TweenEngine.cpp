#include "TweenEngine.h"

#include <fmt/core.h>


void jul::TweenEngine::Update()
{
    while(not m_QueuedTweens.empty())
    {
        auto& [tween, target] = m_QueuedTweens.front();
        m_ActiveTweens.emplace_back(std::make_unique<TweenInstance>(std::move(tween), target));
        m_QueuedTweens.pop();
    }

    for(auto iterator = m_ActiveTweens.begin(); iterator != m_ActiveTweens.end();)
    {
        if((*iterator)->IsDecommisioned())
        {
            iterator = m_ActiveTweens.erase(iterator);
        }
        else
        {
            (*iterator)->Update();
            ++iterator;
        }
    }
}

bool jul::TweenEngine::HasActiveTweens(Object* target)
{
    return std::ranges::any_of(GetInstance().m_ActiveTweens,
                               [target](auto& tween) { return target == tween->GetTarget(); });
}

const std::vector<std::unique_ptr<jul::TweenInstance> >& jul::TweenEngine::GetAllActiveTweens()
{
    return GetInstance().m_ActiveTweens;
}


void jul::TweenEngine::Start(Tween&& tween, Object* target)
{
    if(target == nullptr)
        throw std::runtime_error("Tween started with nullptr");

    // Before starting we check if the target is not already getting destroyed
    // this is to prevent starting a tween in a destroy of another tween
    if(target->IsBeingDestroyed())
    {
        if(tween.invokeWhenDestroyed and tween.onEnd)
            tween.onEnd();

        return;
    }

    // Instantly stop with 0 time
    // In senarios where the user might input a variable time
    if(tween.delay + tween.duration <= 0)
    {
        if(tween.onEnd)
            tween.onEnd();

        return;
    }

    GetInstance().m_QueuedTweens.emplace(std::move(tween), target);
}

void jul::TweenEngine::Start(const Tween& tween, Object* target)
{
    Tween tweenCopy = tween;
    Start(std::move(tweenCopy), target);
}

void jul::TweenEngine::Cancel(Object* target)
{
    for(auto& tween : GetInstance().m_ActiveTweens)
        if(tween->GetTarget() == target)
            tween->Cancel();
}
