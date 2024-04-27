#include "Sound.h"

#include <soloud.h>
#include <soloud_wav.h>

jul::Sound::Sound() { m_SoLoud.init(); }

jul::Sound::~Sound() { m_SoLoud.deinit(); }

void jul::Sound::PlaySound()
{
    SoLoud::Wav sample;
    sample.load("./Assets/SFX/Bubble Bobble SFX (17).wav");  // Load a wave file
    m_SoLoud.play(sample, 1.0f);
}
