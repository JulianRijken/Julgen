#pragma once
#include <soloud.h>

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
        SoLoud::Soloud m_SoLoud;
    };
}  // namespace jul
