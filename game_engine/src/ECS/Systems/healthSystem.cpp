#include "ECS/Systems/healthSystem.hpp"

namespace RealEngine {
void HealthSystem::update(Registry& registry, float deltaTime) {
    auto entities = registry.view<Health>();

    for (auto entity : entities) {
        auto* health = registry.get_component<Health>(entity);

        if (health->regeneration) {
            if (health->regenerationTimer <= 0.0f) {
                health->health += health->regenerationRate;
                if (health->health > health->maxHealth) {
                    health->health = health->maxHealth;
                }
                health->regenerationTimer = health->regenerationCooldown;
            } else {
                health->regenerationTimer -= 1.0f * deltaTime;
            }
        }
        if (health->health <= 0) {
            registry.kill_entity(entity);
        }
    }
}
}  // namespace RealEngine