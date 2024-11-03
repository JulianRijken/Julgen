#include <fmt/core.h>
#include <Resources.h>
#include <soloud.h>
#include <soloud_wav.h>

#include "GameTime.h"
#include "Sound_System.h"

///////////////////////
/// WAVE file setup ///
///////////////////////
namespace jul
{
    class SoundWave::SoundWave_Impl
    {
    public:
        SoundWave_Impl(const std::string& fullPath) { m_Wave.load(fullPath.c_str()); }

        SoLoud::Wav* GetWave() { return &m_Wave; }

    private:
        SoLoud::Wav m_Wave;
    };

}  // namespace jul

jul::SoundWave::SoundWave(const std::string& fullPath) :
    m_SoundWaveImplUPtr(std::make_unique<SoundWave_Impl>(fullPath))
{
}

jul::SoundWave::~SoundWave() = default;  // Required for implementation

/////////////////////////////
/// System implementation ///
/////////////////////////////
namespace jul
{
    class Sound_System::System_Impl
    {
    public:

        System_Impl() { soLoudEngine.init(); }

        ~System_Impl()
        {
            soLoudEngine.deinit();
        }

        void PlaySound(int soundType)
        {
            const SoundWave* sampleToPlay{ Resources::GetSound(soundType) };
            auto handle = soLoudEngine.play(*sampleToPlay->GetImpl().GetWave(), 0.3f);  // TODO: Pass volume
            soLoudEngine.setRelativePlaySpeed(handle, GameTime::GetTimeScale<float>());
        }


        // Relies on member initialization order!
        // Soloud Engine needs to go out of scope last as the jthread needs it
        SoLoud::Soloud soLoudEngine;
    };
}  // namespace jul

jul::Sound_System::Sound_System() :
    m_SoloudImplUPtr(std::make_unique<System_Impl>())
{
}

void jul::Sound_System::PlaySound(int soundType) { m_SoloudImplUPtr->PlaySound(soundType); }

jul::Sound_System::~Sound_System() = default;  // Requires definition
