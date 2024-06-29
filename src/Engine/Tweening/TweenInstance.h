#ifndef TWEENINSTANCE_H
#define TWEENINSTANCE_H
#include "Event.h"
#include "Object.h"
#include "Tween.h"

namespace jul
{
    class GameObject;

    class TweenInstance final : public IEventListener
    {
    public:
        TweenInstance(Tween&& tween, Object* target);

        [[nodiscard]] bool IsDecommisioned() const { return m_IsDecommissioned; }

        [[nodiscard]] const Tween& GetTween() const { return m_Tween; }

        [[nodiscard]] double GetTime() const { return m_Time; }

        [[nodiscard]] bool IsHalting() const { return m_IsHalting; }


        // Used internaly for tween engine to compare target
        [[nodiscard]] Object* GetTarget() const { return m_Target; }

        void Cancel();
        void Update();

    private:
        void OnTargetDestroyed(Object* object);

        bool m_IsHalting{ false };
        bool m_IsDecommissioned{ false };
        bool m_HasReachedEnd{ false };
        double m_Time{ 0 };

        Object* m_Target{ nullptr };
        Tween m_Tween;
    };
}  // namespace jul
#endif // TWEENINSTANCE_H