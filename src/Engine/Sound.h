#pragma once
#include <soloud.h>
#include <soloud_wav.h>

#include "Service.h"


namespace jul
{
    class Sound final : public Service
    {
    public:
        Sound();
        ~Sound();
        void PlaySound();

    private:
        SoLoud::Wav sample;
        SoLoud::Soloud m_SoLoud;
    };
}  // namespace jul
