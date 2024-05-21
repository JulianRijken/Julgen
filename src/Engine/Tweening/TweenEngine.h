#pragma once
#include <GameObject.h>

#include <queue>

#include "Singleton.h"
#include "Tween.h"
#include "TweenInstance.h"

namespace jul
{
    class Object;

    class TweenEngine : public Singleton<TweenEngine>
    {
    public:
        void Update();

        // TODO: Think about returning a tween handle to stop and pause
        static void Start(Tween&& tween, GameObject* target);
        static void Start(const Tween& tween, GameObject* target);

    private:
        std::vector<std::unique_ptr<TweenInstance>> m_ActiveTweens{};
        std::queue<std::pair<Tween, GameObject*>> m_QueuedTweens{};
    };

}  // namespace jul
