#include "Sound_SoLoud.h"

#include <fmt/core.h>
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
    const std::scoped_lock lock{ m_QueueMutex, m_PlayConditionMutex };
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
            m_PlayCondition.wait(lock, [this] { return not m_QueuedSounds.empty() or not m_IsSoundThreadActive; });
        }

        while(not m_QueuedSounds.empty())
        {
            const int soundIndexToPlay = m_QueuedSounds.front();
            m_QueuedSounds.pop();


            SoLoud::Wav* sampleToPlay{};

            if(m_Samples.contains(soundIndexToPlay))
            {
                sampleToPlay = &m_Samples[soundIndexToPlay];
            }
            else
            {
                m_Samples[soundIndexToPlay].load("./Assets/SFX/The Quest Begins.ogg");
                sampleToPlay = &m_Samples[soundIndexToPlay];
            }


            m_SoLoud.play(*sampleToPlay, 0.3f);
        }


        fmt::println("Sound Thread Loop Back");
    }

    m_SoLoud.deinit();

    fmt::println("Sound Thread Closed");
}
