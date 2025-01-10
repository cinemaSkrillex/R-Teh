/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** HealthSystem
*/

#include "ECS/Systems/HealthSystem.hpp"

namespace RealEngine {
HealthSystem::HealthSystem() {}

void HealthSystem::update(Registry& registry, float deltaTime) {
    auto entities = registry.view<Health>();

    if (entities.empty()) {
        return;
    }
    for (auto entity : entities) {
        auto* health = registry.get_component<Health>(entity);
        if (health->invincibilityTime > 0.0f) {
            health->invincibilityTime -= deltaTime;
        }
        if (health->regenerationTime > 0.0f) {
            if (health->regenerationTimer <= 0.0f) {
                health->amount += health->regenerationRate;
                if (health->amount > health->maxHealth) {
                    health->amount = health->maxHealth;
                }
                health->regenerationTimer = health->regenerationCooldown;
            } else {
                health->regenerationTimer -= deltaTime;
            }
            health->regenerationTime -= deltaTime;
        }
    }
}
}  // namespace RealEngine