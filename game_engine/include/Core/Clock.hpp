#pragma once

#include <SFML/Graphics.hpp>

namespace engine {
class Clock {
  public:
    Clock();
    ~Clock();

    void  restart();
    float getElapsedTime() { return _clock.getElapsedTime().asSeconds(); }
    float getElapsedTimeAsSeconds() { return _clock.getElapsedTime().asSeconds(); }
    float getElapsedTimeAsMilliseconds() { return _clock.getElapsedTime().asMilliseconds(); }
    float getElapsedTimeAsMicroseconds() { return _clock.getElapsedTime().asMicroseconds(); }

  private:
    sf::Clock _clock;
};
} // namespace engine