#include "TweenEngine.h"

void jul::TweenEngine::Update()
{
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
    GetInstance().m_ActiveTweens.emplace_back(std::make_unique<TweenInstance>(std::move(tween), target));
}

void jul::TweenEngine::Start(const Tween& tween, GameObject* target)
{
    Tween tweenCopy = tween;
    GetInstance().m_ActiveTweens.emplace_back(std::make_unique<TweenInstance>(std::move(tweenCopy), target));
}
