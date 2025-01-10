#include "../include/ECS/Systems/DestroySystem.hpp"

namespace RealEngine {

DestroySystem::DestroySystem() {}

static void killEntity(Registry& registry, Entity entity, std::vector<Entity>& destroyedEntities) {
    if (registry.is_valid(entity)) {
        destroyedEntities.push_back(entity);
        registry.kill_entity(entity);
    }
}

static bool autoDestroy(Registry& registry, Entity entity, float deltaTime,
                        std::vector<Entity>& destroyedEntities) {
    auto* destructible = registry.get_component<AutoDestructible>(entity);

    if (destructible) {
        if (destructible->lifeTime > 0) {
            destructible->lifeTime -= deltaTime;
            if (destructible->lifeTime <= 0) {
                if (registry.is_valid(entity)) {
                    killEntity(registry, entity, destroyedEntities);
                    return true;
                }
            }
        }
        if (destructible->kill) {
            if (registry.is_valid(entity)) {
                killEntity(registry, entity, destroyedEntities);
                return true;
            }
        }
        if (destructible->killCondition) {
            destructible->kill = true;
        }
    }
    return false;
}

static bool healthDestroy(Registry& registry, Entity entity,
                          std::vector<Entity>& destroyedEntities) {
    auto* health = registry.get_component<Health>(entity);

    if (health) {
        if (health->amount <= 0) {
            if (registry.is_valid(entity)) {
                killEntity(registry, entity, destroyedEntities);
                return true;
            }
        }
    }
    return false;
}

std::vector<Entity> DestroySystem::update(Registry& registry, float deltaTime) {
    std::vector<Entity> destroyedEntities;
    auto                entities = registry.view<>();

    if (entities.empty()) {
        return destroyedEntities;
    }
    for (auto entity : entities) {
        if (autoDestroy(registry, entity, deltaTime, destroyedEntities)) continue;
        if (healthDestroy(registry, entity, destroyedEntities)) continue;
    }
    return destroyedEntities;
}
}  // namespace RealEngine