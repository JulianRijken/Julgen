#pragma once
#include <GameObject.h>

#include <queue>

#include "Singleton.h"
#include "Tween.h"
#include "TweenInstance.h"

namespace jul
{
    class Object;

    class TweenEngine final : public Singleton<TweenEngine>
    {
    public:
        void Update();

        [[nodiscard]] static bool HasActiveTweens(Object* target);

        // Can be used for debugging
        [[nodiscard]] static const std::vector<std::unique_ptr<TweenInstance>>& GetAllActiveTweens();

        // TODO: Think about returning a tween handle to stop and pause
        static void Start(Tween&& tween, Object* target);
        static void Start(const Tween& tween, Object* target);
        static void Cancel(Object* target);

    private:
        std::vector<std::unique_ptr<TweenInstance>> m_ActiveTweens{};
        std::queue<std::pair<Tween, Object*>> m_QueuedTweens{};
    };

}  // namespace jul
