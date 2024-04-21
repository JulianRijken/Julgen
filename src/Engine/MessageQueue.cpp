#include "MessageQueue.h"

void jul::MessageQueue::Broadcast(const Message& message) { m_Messages.push(message); }

void jul::MessageQueue::Dispatch()
{
    while (not m_Messages.empty())
    {
        const Message message = m_Messages.front();
        m_Messages.pop();

        auto listeners = m_MessageListeners.equal_range(message.type);

        for(auto it = listeners.first; it != listeners.second; ++it)
            it->second.second(message);
    }
}