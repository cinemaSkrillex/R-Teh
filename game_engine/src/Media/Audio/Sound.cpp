#include "../include/Media/Audio/Sound.hpp"

namespace RealEngine {
Sound::Sound(const std::string filePath) { loadFile(filePath); }

Sound::~Sound() {}

void Sound::loadFile(const std::string filePath) {
    _buffer.loadFromFile(filePath);
    _sound.setBuffer(_buffer);
}

void Sound::play() { _sound.play(); }

void Sound::stop() { _sound.stop(); }

void Sound::setVolume(float volume) {
    _volume = volume;
    _sound.setVolume(volume);
}

void Sound::setPitch(float pitch) { _sound.setPitch(pitch); }
}  // namespace RealEngine