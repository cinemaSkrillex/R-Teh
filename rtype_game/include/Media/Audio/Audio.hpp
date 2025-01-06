#pragma once

#include <SFML/Audio.hpp>
#include <string>

namespace RealEngine {
class IAudio {
   public:
    IAudio()                                          = default;
    ~IAudio()                                         = default;
    virtual void play()                               = 0;
    virtual void stop()                               = 0;
    virtual void loadFile(const std::string filePath) = 0;
    virtual void setVolume(float volume)              = 0;
    virtual void setPitch(float pitch)                = 0;
};
}  // namespace RealEngine
