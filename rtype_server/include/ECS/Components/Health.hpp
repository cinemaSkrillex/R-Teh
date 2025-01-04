#pragma once

namespace RealEngine {
struct Health {
    int   amount;
    int   maxHealth;
    int   damage               = 0;      // all damage taken between each frame
    float regenerationRate     = 0;      // amount of health regenerated per timer
    float regenerationCooldown = 0;      // time between regenerations (const)
    float regenerationTimer    = 0;      // timer between two regenerations
    float regenerationTime     = 0;      // time of regeneration
    float invincibilityTime    = 0;      // time of invincibility
    bool  showTakenDamage      = false;  // display a text with the amount of damage taken
};
}  // namespace RealEngine