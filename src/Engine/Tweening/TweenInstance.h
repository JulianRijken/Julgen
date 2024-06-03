#pragma once
#include "Event.h"
#include "Object.h"
#include "Tween.h"

namespace jul
{
    class GameObject;

    class TweenInstance : public IEventListener
    {
    public:
        TweenInstance(Tween&& tween, Object* target);

        [[nodiscard]] bool IsDecommisioned() const { return m_IsDecommissioned; }

        // Used internaly for tween engine to compare target
        [[nodiscard]] Object* GetTarget() const { return m_Target; }

        void Cancel();

        void OnTargetDestroyed();

        void Update();

    private:
        bool m_IsHalting{ false };
        bool m_IsDecommissioned{ false };
        bool m_HasReachedEnd{ false };
        double m_Time{};

        Tween m_Tween;
        Object* m_Target{ nullptr };
    };
}  // namespace jul
