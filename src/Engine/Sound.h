#pragma once
#include <soloud.h>
#include <soloud_wav.h>

#include "Service.h"


namespace jul
{
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
