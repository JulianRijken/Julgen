#include "Sound.h"

#include <soloud.h>
#include <soloud_wav.h>

jul::Sound::Sound() { m_SoLoud.init(); }

jul::Sound::~Sound() { m_SoLoud.deinit(); }

void jul::Sound::PlaySound()
{
    sample.load("./Assets/SFX/The Quest Begins.ogg");
    m_SoLoud.play(sample, 0.1f);
}
