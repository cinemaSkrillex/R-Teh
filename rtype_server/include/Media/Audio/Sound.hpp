#pragma once

#include "Audio.hpp"

namespace RealEngine {
class Sound : public IAudio {
  public:
    Sound(const std::string filePath);
    ~Sound();
    void play() override;
    void stop() override;
    void loadFile(const std::string filePath) override;
    void setVolume(float volume) override;
    void setPitch(float pitch) override;

  private:
    sf::Sound       _sound;
    sf::SoundBuffer _buffer;
    float           _volume;
    bool            _isPlaying;
};
} // namespace RealEngine