#pragma once

namespace RealEngine {
struct Health {
    int   health;
    int   maxHealth;
    bool  regeneration;
    float regenerationRate;
    float regenerationCooldown;
    float regenerationTimer;
};
}  // namespace RealEngine