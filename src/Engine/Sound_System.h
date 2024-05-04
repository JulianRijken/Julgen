#pragma once
#include <memory>
#include <string>

#include "Sound.h"

namespace jul
{
    class SoundWave
    {
        class SoundWave_Impl;
        std::unique_ptr<SoundWave_Impl> m_SoundWaveImplUPtr;

    public:
        SoundWave(const std::string& fullPath);
        ~SoundWave();

        // TODO: Is this still valid in hiding the impl?
        SoundWave_Impl& GetImpl() { return *m_SoundWaveImplUPtr; }
    };

    class Sound_System final : public Sound
    {
        class System_Impl;
        std::unique_ptr<System_Impl> m_SoloudImplUPtr;

    public:
        explicit Sound_System();
        ~Sound_System() override;

        Sound_System(Sound_System&&) = delete;
        Sound_System(const Sound_System&) = delete;
        Sound_System& operator=(Sound_System&&) = delete;
        Sound_System& operator=(const Sound_System&) = delete;

        void PlaySound(int soundType) override;
    };
}  // namespace jul
