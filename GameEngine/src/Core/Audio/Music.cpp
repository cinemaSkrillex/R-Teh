#include "../include/Core/Audio/Music.hpp"

namespace RealEngine {
Music::Music(const std::string filePath) {
    loadFile(filePath);
    _replayPoint = sf::seconds(0);
    _music.setLoop(false);
    _volume          = 100.0;
    _fadeOut         = false;
    _fadeOutDuration = 0;
    _stopped         = false;
}

Music::~Music() {}

bool Music::loadFile(const std::string filePath) {
    if (!_music.openFromFile(filePath)) {
        throw std::runtime_error("Failed to load music: " + filePath);
        return false;
    }
    setLoop(true);
    return true;
}

void Music::update() {
    if (_fadeOut) {
        float volume = _music.getVolume();
        if (volume > 0) {
            volume -= 100 * _fadeOutDuration;
            if (volume < 0) volume = 0;
            _music.setVolume(volume);
        } else {
            _fadeOut = false;
            stop();
        }
    }
    if (!_stopped && !isPlaying()) replay();
}

void Music::play() {
    _stopped = false;
    _music.play();
}

void Music::replay() {
    play();
    _music.setPlayingOffset(_replayPoint);
}

void Music::stop() {
    _stopped = true;
    _music.stop();
}

void Music::fadeOut(float duration) {
    if (duration < 0) {
        std::cerr << "Fade out duration must be greater than 0" << std::endl;
        return;
    }
    _fadeOut         = true;
    _fadeOutDuration = duration;
}

bool Music::isPlaying() { return _music.getStatus() == sf::Music::Playing; }

void Music::setReplayPoint(float seconds) {
    if (seconds < 0) {
        std::cerr << "Replay point must be greater than 0" << std::endl;
        return;
    }
    _replayPoint = sf::seconds(seconds);
}

void Music::setVolume(float volume) {
    if (volume < 0 || volume > 100) {
        std::cerr << "Volume must be between 0 and 100" << std::endl;
        return;
    }
    _volume = volume;
    _music.setVolume(volume);
}

void Music::setPitch(float pitch) {
    if (pitch < 0.5 || pitch > 2) {
        std::cerr << "Pitch must be between 0.5 and 2" << std::endl;
        return;
    }
    _music.setPitch(pitch);
}

void Music::setLoop(bool loop) { _music.setLoop(loop); }
}  // namespace RealEngine