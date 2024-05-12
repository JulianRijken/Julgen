#pragma once


#include <iostream>

#include "Sound.h"

namespace jul
{
    class Sound_Null final : public Sound
    {
    public:
        Sound_Null() = default;
        ~Sound_Null() override = default;

        Sound_Null(Sound_Null&&) = delete;
        Sound_Null(const Sound_Null&) = delete;
        Sound_Null& operator=(Sound_Null&&) = delete;
        Sound_Null& operator=(const Sound_Null&) = delete;

        void PlaySound(int soundId) override { std::cerr << "Null Sound Playing " << soundId << std::endl; }

    private:
    };
}  // namespace jul
