#pragma once

#include <algorithm>
#include <functional>
#include <unordered_set>

namespace jul
{
    class EventListener;

    class BaseEvent
    {
    public:
        BaseEvent() = default;
        virtual ~BaseEvent() = default;

        BaseEvent(BaseEvent&&) = delete;
        BaseEvent(const BaseEvent&) = delete;
        BaseEvent& operator=(BaseEvent&&) = delete;
        BaseEvent& operator=(const BaseEvent&) = delete;

        virtual void RemoveListener(EventListener* listener) = 0;
    };

    class EventListener
    {
        template<typename... EventArgs>
        friend class Event;

    public:
        virtual ~EventListener()
        {
            for(auto* event : m_BindedEvents)
                event->RemoveListener(this);
        }

        EventListener(EventListener&&) = delete;
        EventListener(const EventListener&) = delete;
        EventListener& operator=(EventListener&&) = delete;
        EventListener& operator=(const EventListener&) = delete;

    protected:
        EventListener() = default;

    private:
        void AddEvent(BaseEvent* event) { m_BindedEvents.insert(event); }

        void RemoveEvent(BaseEvent* event) { m_BindedEvents.erase(event); }

        std::unordered_set<BaseEvent*> m_BindedEvents;
    };

    template<typename... EventArgs>
    class Event final : public BaseEvent
    {
        using FunctionBind = std::pair<void*, std::function<void(EventArgs...)>>;

    public:
        Event() = default;

        ~Event() override
        {
            for(auto* eventListener : m_EventListeners)
                eventListener->RemoveEvent(this);
        }

        Event(Event&&) = delete;
        Event(const Event&) = delete;
        Event& operator=(Event&&) = delete;
        Event& operator=(const Event&) = delete;

        template<typename Function>
        void AddListener(Function function)
        {
            m_FunctionBinds.emplace_back(nullptr, [function](EventArgs... args) { function(args...); });
        }

        template<typename ObjectType>
            requires std::derived_from<ObjectType, EventListener>
        void AddListener(ObjectType* object, void (ObjectType::*memberFunction)(EventArgs...))
        {
            auto* listener = static_cast<EventListener*>(object);
            listener->AddEvent(this);
            m_EventListeners.insert(listener);

            m_FunctionBinds.emplace_back(
                listener, [object, memberFunction](EventArgs... args) { (object->*memberFunction)(args...); });
        }

        template<typename... Args>
        void Invoke(Args&&... args)
        {
            for(auto&& listenerFunction : m_FunctionBinds)
                listenerFunction.second(args...);
        }


    private:
        void RemoveListener(EventListener* listener) override
        {
            m_EventListeners.erase(listener);

            m_FunctionBinds.erase(std::remove_if(m_FunctionBinds.begin(),
                                                 m_FunctionBinds.end(),
                                                 [listener](const FunctionBind& bind)
                                                 { return bind.first == static_cast<void*>(listener); }),
                                  m_FunctionBinds.end());
        }

        std::vector<FunctionBind> m_FunctionBinds{};
        std::unordered_set<EventListener*> m_EventListeners{};
    };
}  // namespace jul
