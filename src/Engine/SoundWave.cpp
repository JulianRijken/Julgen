#include "SoundWave.h"

jul::SoundWave::SoundWave(const std::string& path) { m_Wave.load(path.c_str()); }
