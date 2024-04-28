#pragma once
#include <soloud.h>
#include <soloud_wav.h>

#include <string>

namespace jul
{

    class SoundWave
    {
    public:
        SoundWave(const std::string& fullPath);

        SoLoud::Wav& GetSound() { return m_Wave; }

    private:
        SoLoud::Wav m_Wave;
    };
}  // namespace jul
