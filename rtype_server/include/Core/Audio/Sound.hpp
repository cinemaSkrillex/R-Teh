#pragma once

#include "Audio.hpp"

namespace RealEngine {
class Sound : public IAudio {
   public:
    Sound(const std::string filePath);
    Sound();
    ~Sound();
    void            play() override;
    void            stop() override;
    bool            loadFile(const std::string filePath) override;
    void            setVolume(float volume) override;
    void            setPitch(float pitch) override;
    sf::Sound       getSound() { return _sound; }
    sf::SoundBuffer getBuffer() { return _buffer; }

   private:
    sf::Sound       _sound;
    sf::SoundBuffer _buffer;
    float           _volume;
    bool            _isPlaying = false;
};
}  // namespace RealEngine