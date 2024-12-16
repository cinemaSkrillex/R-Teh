#include "../include/ECS/Systems/DestructibleSystem.hpp"

namespace RealEngine {

DestructibleSystem::DestructibleSystem() {}

std::vector<Entity> DestructibleSystem::update(Registry& registry, float deltaTime) {
    std::vector<Entity> destroyedEntities;
    auto entities = registry.view<AutoDestructible>();
    if (entities.empty()) {
        return destroyedEntities;
    }
    for (auto entity : entities) {
        auto* destructible = registry.get_component<AutoDestructible>(entity);
        if (!destructible) {
            continue;
        }
        if (destructible->lifeTime > 0) {
            destructible->lifeTime -= deltaTime;
            if (destructible->lifeTime <= 0) {
                // std::cout << "Entity " << entity << " destroyed" << std::endl;  // TODO: remove
                if (registry.is_valid(entity)) {
                    destroyedEntities.push_back(entity);
                    registry.kill_entity(entity);
                    continue;
                }
            }
        }
        // if (destructible->killCondition()) {
        //     registry.kill_entity(entity);
        // }
    }
    return destroyedEntities;
}
}  // namespace RealEngine