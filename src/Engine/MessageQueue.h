#pragma once

#include <any>
#include <functional>
#include <queue>
#include <utility>


namespace jul
{
    struct Message
    {
        int id;
        std::vector<std::any> arguments;
    };

    class MessageQueue
    {
        using Listener = std::pair<void*, std::function<void(const Message&)>>;

    public:
        static void Broadcast(const Message& message);

        template<typename MessageType>
        static void Broadcast(MessageType messageType, std::vector<std::any>&& args = {})
        {
            m_Messages.push({ static_cast<int>(messageType), std::move(args) });
        }

        template<typename ObjectType, typename MessageType>
        static void AddListener(MessageType eventType, ObjectType* object,
                                void (ObjectType::*memberFunction)(const Message&))
        {
            m_MessageListeners.insert({
                static_cast<int>(eventType),
                {object, [=](const Message& message) { (object->*memberFunction)(message); }}
            });
        }

        template<typename ObjectType>
        static void RemoveListenerInstance(ObjectType* object)
        {
            std::erase_if(m_MessageListeners, [=](const auto& other) { return other.second.first == object; });
        }

        static void Dispatch();

        static inline std::unordered_multimap<int, Listener> m_MessageListeners{};
        static inline std::queue<Message> m_Messages{};
    };

}  // namespace jul
