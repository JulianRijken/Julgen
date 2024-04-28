#pragma once
#include <fmt/core.h>

#include <memory>

#include "Sound.h"

namespace jul
{
    template<typename SoundSystemType>
        requires std::derived_from<SoundSystemType, Sound>
    class Sound_Logging final : public Sound
    {
    public:
        explicit Sound_Logging() :
            m_SoundSystemPtr(std::make_unique<SoundSystemType>())
        {
        }

        explicit Sound_Logging(std::unique_ptr<SoundSystemType> soundSystem) :
            m_SoundSystemPtr(std::move(soundSystem))
        {
        }

        ~Sound_Logging() override = default;

        Sound_Logging(Sound_Logging&&) = delete;
        Sound_Logging(const Sound_Logging&) = delete;
        Sound_Logging& operator=(Sound_Logging&&) = delete;
        Sound_Logging& operator=(const Sound_Logging&) = delete;

        void PlaySound(int soundType) override
        {
            m_SoundSystemPtr->PlaySound(soundType);
            fmt::println("Sound Played");
        }

        std::unique_ptr<SoundSystemType> ReleaseSoundSystem() { return std::move(m_SoundSystemPtr); }

    private:
        std::unique_ptr<SoundSystemType> m_SoundSystemPtr;
    };
}  // namespace jul
