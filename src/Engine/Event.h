#pragma once

#include <algorithm>
#include <functional>

namespace jul
{
    // Event can be used as a asynchronous direct way of communicating between classes
    // without directly depending on each other
    template<typename... EventArgs>
    class Event final
    {
        using FunctionType = std::function<void (EventArgs...)>;
        using Listener = std::pair<void *, FunctionType>;

    public:
        Event() = default;

        template<typename Function>
        void AddListener(Function function)
        {
            m_ListenerFunctions.emplace_back(nullptr, [function](EventArgs... args) { function(args...); });
        }

        template<typename ObjectType>
        void AddListener(ObjectType* object, void (ObjectType::*memberFunction)(EventArgs...))
        {
            m_ListenerFunctions.emplace_back(
                object, [object, memberFunction](EventArgs... args) { (object->*memberFunction)(args...); });
        }

        template<typename... Args>
        void Invoke(Args&&... args)
        {
            for (auto&& listenerFunction : m_ListenerFunctions)
                listenerFunction.second(args...);
        }

        template<typename ObjectType>
        void RemoveListenerInstance(ObjectType* object)
        {
            std::ranges::remove_if(m_ListenerFunctions,[=](Listener function){
                return function.first == object;
            });
        }

    private:
        std::vector<Listener> m_ListenerFunctions{};
    };
}
