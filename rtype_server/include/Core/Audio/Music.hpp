#pragma once

#include "Audio.hpp"

namespace RealEngine {
class Music : public IAudio {
   public:
    Music(const std::string filePath);
    ~Music();
    void update();
    void play() override;
    void stop() override;
    void fadeOut(float duration);
    void replay();
    void setReplayPoint(float seconds);
    bool isPlaying();
    void loadFile(const std::string filePath) override;
    void setVolume(float volume) override;
    void setPitch(float pitch) override;
    void setLoop(bool loop);

   private:
    sf::Music _music;
    sf::Time  _replayPoint;
    float     _volume;
    bool      _fadeOut;
    float     _fadeOutDuration;
    bool      _stopped;
    bool      _isPlaying;
};
}  // namespace RealEngine