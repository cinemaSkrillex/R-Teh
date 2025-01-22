#include "../include/Core/Audio/Sound.hpp"

namespace RealEngine {
Sound::Sound(const std::string filePath) { loadFile(filePath); }

Sound::Sound() {}

Sound::~Sound() {}

bool Sound::loadFile(const std::string filePath) {
    if (!_buffer.loadFromFile(filePath)) {
        throw std::runtime_error("Failed to load sound: " + filePath);
        return false;
    }
    _sound.setBuffer(_buffer);
    return true;
}

void Sound::play() {
    if (_isPlaying) {
        stop();
    }
    _sound.play();
    _isPlaying = true;
}

void Sound::stop() {
    _sound.stop();
    _isPlaying = false;
}

void Sound::setVolume(float volume) {
    if (volume < 0 || volume > 100) {
        std::cerr << "Volume must be between 0 and 100" << std::endl;
        return;
    }
    _volume = volume;
    _sound.setVolume(volume);
}

void Sound::setPitch(float pitch) {
    if (pitch < 0.5 || pitch > 2) {
        std::cerr << "Pitch must be between 0.5 and 2" << std::endl;
        return;
    }
    _sound.setPitch(pitch);
}
}  // namespace RealEngine