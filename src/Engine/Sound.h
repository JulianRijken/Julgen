#pragma once
#include <memory>
#include <string>

#include "Service.h"


namespace jul
{
    class SoundWave
    {
        struct SoundWave_Impl;
        std::unique_ptr<SoundWave_Impl> m_SoundWaveImplUPtr;

    public:
        SoundWave(const std::string& fullPath);
        ~SoundWave();

        SoundWave_Impl* GetImpl() { return m_SoundWaveImplUPtr.get(); }
    };

    class Sound : public Service
    {
    public:
        explicit Sound() = default;
        ~Sound() override = default;

        Sound(Sound&&) = delete;
        Sound(const Sound&) = delete;
        Sound& operator=(Sound&&) = delete;
        Sound& operator=(const Sound&) = delete;

        virtual void PlaySound(int soundType) = 0;
    };
}  // namespace jul
