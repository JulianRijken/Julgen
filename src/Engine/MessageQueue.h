#pragma once

#include <any>
#include <functional>
#include <queue>

namespace jul
{

    enum class MessageType
    {
        PlayerScoreChanged
    };

    struct Message
    {
        MessageType type;
        std::vector<std::any> arguments;
    };

    class MessageQueue
    {
        using Listener = std::pair<void*, std::function<void(const Message&)>>;

    public:
        static void Broadcast(const Message& message);

        template<typename ObjectType>
        static void AddListener(MessageType eventType, ObjectType* object,
                                void (ObjectType::*memberFunction)(const Message&))
        {
            m_MessageListeners.insert({
                eventType, {object, [=](const Message& message) { (object->*memberFunction)(message); }}
            });
        }

        template<typename ObjectType>
        static void RemoveListenerInstance(ObjectType* object)
        {
            std::erase_if(m_MessageListeners, [=](const auto& other) { return other.second.first == object; });
        }

        static void Dispatch();

        static inline std::unordered_multimap<MessageType, Listener> m_MessageListeners{};
        static inline std::queue<Message> m_Messages{};
    };

}  // namespace jul
