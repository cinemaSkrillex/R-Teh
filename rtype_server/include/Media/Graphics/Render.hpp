#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>

namespace RealEngine {
class IRender {
  public:
    IRender()                                  = default;
    ~IRender()                                 = default;
    virtual void disappear()                   = 0;
    virtual void setPosition(float x, float y) = 0;
    virtual void setScale(float x, float y)    = 0;
    virtual void setRotation(float angle)      = 0;
};
} // namespace RealEngine