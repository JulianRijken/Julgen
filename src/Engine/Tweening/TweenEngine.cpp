#include "TweenEngine.h"

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

void jul::TweenEngine::Start(Tween&& tween, GameObject* target)
{
    GetInstance().m_QueuedTweens.emplace(std::move(tween), target);
}

void jul::TweenEngine::Start(const Tween& tween, GameObject* target)
{
    Tween tweenCopy = tween;
    GetInstance().m_QueuedTweens.emplace(std::move(tweenCopy), target);
}