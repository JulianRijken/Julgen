#include "Sound_SoLoud.h"

#include <soloud.h>
#include <soloud_wav.h>

jul::Sound_SoLoud::Sound_SoLoud() { m_SoLoud.init(); }

jul::Sound_SoLoud::~Sound_SoLoud() { m_SoLoud.deinit(); }

void jul::Sound_SoLoud::PlaySound()
{
    m_Sample.load("./Assets/SFX/The Quest Begins.ogg");
    m_SoLoud.play(m_Sample, 0.1f);
}
