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

        void AddListener(Listener listener)
        {
            m_ListenerFunctions.push_back({nullptr, listener});
        }

        template<typename ObjectType>
        void AddListener(ObjectType* object,  void(ObjectType::* memberFunction)(EventArgs...))
        {
            m_ListenerFunctions.emplace_back(object,[=](EventArgs... args) {
                (object->*memberFunction)(args...);
            });
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

        // HINT: RemoveListener() not implemented, out of scope for project
        //       Implementation is delayed as it requires a way to compare member functions,
        //       this is not trivial and requirements a token system of some sorts
        //       this is also left out as Tom recommend to not overcomplicate the project when not needed
        //
        // EXAMPLE: A player (observer) listens to an enemy death event (subject)
        //          When enemy dies before the player: the player gets a notification and can react to it
        //          When player dies before the enemy: The enemy has no way of knowing and here we need RemoveListener() to be called by the player
        //          Unity solves this with C# inline null checks
        //          Unreal engine uses a garbage collection system and does not need remove listener
        //
        //          As long as the observer does not get destroyed before the subject, this works.
        //
        //          RemoveListenerInstance() can be used to solve this but does not allow for removing individual functions

    private:
        std::vector<Listener> m_ListenerFunctions{};
    };
}
