#include "MessageQueue.h"

void jul::MessageQueue::Broadcast(const Message& message) { g_Messages.push(message); }

void jul::MessageQueue::Dispatch()
{
    while(not g_Messages.empty())
    {
        const Message message = g_Messages.front();
        g_Messages.pop();

        auto listenersIterator = g_MessageListeners.equal_range(message.id);

        for(auto it = listenersIterator.first; it != listenersIterator.second; ++it)
            it->second.second(message);
    }
}
