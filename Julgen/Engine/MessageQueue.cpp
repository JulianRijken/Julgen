#include "MessageQueue.h"

void MessageQueue::Broadcast(const Message& message)
{
    m_Messages.push(message);
}

void MessageQueue::Dispatch()
{
    while (not m_Messages.empty())
    {
        Message message = m_Messages.front();
        m_Messages.pop();

        auto listeners = m_MessageListeners.equal_range(message.type);

        for(auto it = listeners.first; it != listeners.second; ++it)
            it->second.second(message);
    }
}
