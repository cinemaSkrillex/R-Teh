#pragma once

#include <SFML/Graphics.hpp>

namespace RealEngine {

struct Damage {
    float amount;
    //TODO maybe, have a projectile amount and a body amount
    bool  effect         = false;  // damage during time
    float effectDuration = 0;      // duration of the effect
    float effectInterval = 0;      // interval between each damage
    bool  stealLife      = false;  // each damage taken give tha half of the amount to the attacker
};
}  // namespace RealEngine