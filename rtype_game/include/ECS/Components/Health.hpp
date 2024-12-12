#pragma once

namespace RealEngine {
struct Health {
    int   amount;
    int   maxHealth;
    int   damage               = 0;
    float regenerationRate     = 0;
    float regenerationCooldown = 0;
    float regenerationTimer    = 0;
    float regenerationTime     = 0;
    float invincibilityTime    = 0;
};
}  // namespace RealEngine