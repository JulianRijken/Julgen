#pragma once
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

        void PlaySound() override;

    private:
        SoLoud::Wav m_Sample;
        SoLoud::Soloud m_SoLoud;
    };
}  // namespace jul
