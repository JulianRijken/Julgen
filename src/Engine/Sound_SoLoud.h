#pragma once
#include <condition_variable>
#include <queue>
#include <thread>

#include "Sound.h"

namespace jul
{
    class Sound_SoLoud final : public Sound
    {
    public:
        explicit Sound_SoLoud();
        ~Sound_SoLoud() override;

        Sound_SoLoud(Sound_SoLoud&&) = delete;
        Sound_SoLoud(const Sound_SoLoud&) = delete;
        Sound_SoLoud& operator=(Sound_SoLoud&&) = delete;
        Sound_SoLoud& operator=(const Sound_SoLoud&) = delete;

        void PlaySound(int soundType) override;


    private:
        void SoundThread();

        SoLoud::Soloud m_SoLoud;

        std::condition_variable m_PlayCondition;
        std::mutex m_PlayConditionMutex;
        std::mutex m_QueueMutex;

        std::queue<int> m_QueuedSounds{};  // Using a set to void duplicates

        bool m_IsSoundThreadActive;
        std::jthread m_SoundThread;
    };
}  // namespace jul
