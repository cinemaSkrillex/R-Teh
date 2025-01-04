/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** HealthSystem
*/

#include "ECS/Systems/HealthSystem.hpp"

namespace RealEngine {
HealthSystem::HealthSystem() {}

// void HealthSystem::update(Registry& registry, float deltaTime) {
std::vector<Entity> HealthSystem::update(Registry& registry, float deltaTime) {
    std::vector<Entity> destroyedEntities;
    auto                entities = registry.view<Health>();

    if (entities.empty()) {
        return destroyedEntities;
    }
    for (auto entity : entities) {
        auto* health = registry.get_component<Health>(entity);
        if (health->invincibilityTime > 0.0f) {
            health->invincibilityTime -= deltaTime;
            if (health->regenerationRate < 0) {
                health->regenerationRate = 0;
                health->regenerationTime = 0;
            }
        } else {
            health->amount -= health->damage;
            health->damage = 0;
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
        if (health->amount <= 0) {
            destroyedEntities.push_back(entity);
            registry.kill_entity(entity);
        }
    }
    return destroyedEntities;
}
}  // namespace RealEngine