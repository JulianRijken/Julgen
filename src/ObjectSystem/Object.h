#ifndef OBJECT_H
#define OBJECT_H
#include <string>

#include "Event.h"


namespace jul
{
    class Object
    {
    public:
        virtual ~Object();

        Object(const Object&) = delete;
        Object(Object&&) noexcept = delete;
        Object& operator=(const Object&) = delete;
        Object& operator=(Object&&) noexcept = delete;

        [[nodiscard]] const std::string& GetName() const { return m_Name; }

        [[nodiscard]] bool IsBeingDestroyed() const { return m_BeingDestroyed; }

        [[nodiscard]] Event<Object*>& GetOnDestroyedEvent() { return m_OnDestroyedEvent; }

        void DestroyDelayed(double delay);

        virtual void Destroy();

    protected:
        Object(std::string name = "Object");

    private:
        bool m_BeingDestroyed{false};
        std::string m_Name{};
        Event<Object*> m_OnDestroyedEvent{};
    };
}  // namespace jul
#endif // OBJECT_H