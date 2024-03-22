#pragma once

#include <functional>

namespace jul
{
    template<typename... EventArgs>
    class Event final
    {
        typedef std::function<void(EventArgs...)> FunctionType;

    public:
        Event() = default;

        ~Event()
        {
            m_ListenerFunctions.clear();
        }

        Event(Event&&) = delete;
        Event(const Event&) = delete;
        Event& operator=(Event&&) = delete;
        Event& operator=(const Event&) = delete;

        void AddListener(FunctionType listener)
        {
            m_ListenerFunctions.push_back(listener);
        }

        template<typename ObjectType>
        void AddListener(ObjectType* object,  void(ObjectType::* memberFunction)(EventArgs...))
        {
            m_ListenerFunctions.emplace_back([&](EventArgs... args) {
                (object->*memberFunction)(args...);
            });
        }

        template<typename... Args>
        void Invoke(Args&&... args)
        {
            for (auto&& listenerFunction : m_ListenerFunctions)
                listenerFunction(args...);
        }

    private:

        std::vector<FunctionType> m_ListenerFunctions{};
    };
}
