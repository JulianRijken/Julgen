#pragma once
#include "Event.h"
#include "Tween.h"

namespace jul
{
    class GameObject;

    class TweenInstance : public EventListener
    {
    public:
        TweenInstance(Tween&& tween, GameObject* target);

        [[nodiscard]] bool IsDecommisioned() const { return m_IsDecommissioned; }

        void OnTargetDestroyed();

        void Update();

    private:
        bool m_IsDecommissioned{ false };
        bool m_HasReachedEnd{ false };
        double m_Time{};

        Tween m_Tween;
        GameObject* m_Target{ nullptr };
    };
}  // namespace jul
