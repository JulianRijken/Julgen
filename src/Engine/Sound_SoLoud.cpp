#include "Sound_SoLoud.h"

#include <fmt/core.h>
#include <ResourceManager.h>
#include <soloud.h>
#include <soloud_wav.h>

#include <mutex>

jul::Sound_SoLoud::Sound_SoLoud() :
    m_IsSoundThreadActive{ true },
    m_SoundThread{ &Sound_SoLoud::SoundThread, this }
{
}

void jul::Sound_SoLoud::PlaySound(int soundType)
{
    const std::scoped_lock lock{ m_PlayConditionMutex, m_QueueMutex };
    m_QueuedSounds.push(soundType);
    m_PlayCondition.notify_all();
}


jul::Sound_SoLoud::~Sound_SoLoud()
{
    {
        const std::scoped_lock lock{ m_PlayConditionMutex };
        m_IsSoundThreadActive = false;
        m_PlayCondition.notify_all();
    }
}

void jul::Sound_SoLoud::SoundThread()
{
    m_SoLoud.init();

    while(m_IsSoundThreadActive)
    {
        {
            std::unique_lock<std::mutex> lock(m_PlayConditionMutex);
            m_PlayCondition.wait(lock,
                                 [this]
                                 {
                                     const std::scoped_lock queueLock{ m_QueueMutex };
                                     return not m_QueuedSounds.empty() or not m_IsSoundThreadActive;
                                 });
        }

        if(not m_IsSoundThreadActive)
            break;


        // Manually locking to keep members in scope
        m_QueueMutex.lock();
        const int soundIndexToPlay = m_QueuedSounds.front();
        m_QueuedSounds.pop();
        m_QueueMutex.unlock();

        SoundWave* sampleToPlay{ ResourceManager::GetSound(soundIndexToPlay) };

        m_SoLoud.play(sampleToPlay->GetSound(), 0.3f);


        fmt::println("Sound Thread Loop Back");
    }

    m_SoLoud.deinit();

    fmt::println("Sound Thread Closed");
}
