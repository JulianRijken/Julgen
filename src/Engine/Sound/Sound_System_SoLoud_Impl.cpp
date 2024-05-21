#include <fmt/core.h>
#include <ResourceManager.h>
#include <soloud.h>
#include <soloud_wav.h>

#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

#include "Sound_System.h"

///////////////////////
/// WAVE file setup ///
///////////////////////
namespace jul
{
    class SoundWave::SoundWave_Impl
    {
    public:
        SoundWave_Impl(const std::string& fullPath) { m_Wave.load(fullPath.c_str()); }

        SoLoud::Wav* GetWave() { return &m_Wave; }

    private:
        SoLoud::Wav m_Wave;
    };

}  // namespace jul

jul::SoundWave::SoundWave(const std::string& fullPath) :
    m_SoundWaveImplUPtr(std::make_unique<SoundWave_Impl>(fullPath))
{
}

jul::SoundWave::~SoundWave() = default;  // Required for implementation

/////////////////////////////
/// System implementation ///
/////////////////////////////
namespace jul
{
    class Sound_System::System_Impl
    {
    public:
        System_Impl() :
            m_IsSoundThreadActive{ true },
            m_SoundThread{ &System_Impl::SoundThread, this }
        {
        }

        ~System_Impl()
        {
            const std::scoped_lock lock{ m_PlayConditionMutex };
            m_IsSoundThreadActive = false;
            m_PlayCondition.notify_all();
        }

        void PlaySound(int soundType)
        {
            const std::scoped_lock lock{ m_PlayConditionMutex, m_QueueMutex };
            m_QueuedSounds.push(soundType);
            m_PlayCondition.notify_all();
        }


    private:
        void SoundThread()
        {
            soLoudEngine.init();

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

                const SoundWave* sampleToPlay{ ResourceManager::GetSound(soundIndexToPlay) };
                soLoudEngine.play(*sampleToPlay->GetImpl().GetWave(), 0.3f);  // TODO: Pass volume
            }

            soLoudEngine.deinit();
        }

        // Relies on member initialization order!
        // Soloud Engine needs to go out of scope last as the jthread needs it
        SoLoud::Soloud soLoudEngine;

        std::condition_variable m_PlayCondition;
        std::mutex m_PlayConditionMutex;
        std::mutex m_QueueMutex;

        std::queue<int> m_QueuedSounds{};  // Using a set to void duplicates

        bool m_IsSoundThreadActive;
        std::jthread m_SoundThread;
    };
}  // namespace jul

jul::Sound_System::Sound_System() :
    m_SoloudImplUPtr(std::make_unique<System_Impl>())
{
}

void jul::Sound_System::PlaySound(int soundType) { m_SoloudImplUPtr->PlaySound(soundType); }

jul::Sound_System::~Sound_System() = default;  // Requires definition
