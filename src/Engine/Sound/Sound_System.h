#ifndef SOUND_SYSTEM_H
#define SOUND_SYSTEM_H
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
        [[nodiscard]] SoundWave_Impl& GetImpl() const { return *m_SoundWaveImplUPtr; }
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
#endif // SOUND_SYSTEM_H