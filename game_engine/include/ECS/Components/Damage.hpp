#pragma once

#include <SFML/Graphics.hpp>

namespace RealEngine {

struct Damage {
    float amount;
    bool  effect         = false;
    float effectDuration = 0;
    float effectInterval = 0;
    bool  stealLife      = false;
};
}  // namespace RealEngine