#include "../include/ECS/Systems/DestructibleSystem.hpp"

namespace RealEngine {

DestructibleSystem::DestructibleSystem() {}

void DestructibleSystem::update(Registry& registry, float deltaTime) {
    auto entities = registry.view<AutoDestructible>();

    for (auto entity : entities) {
        auto* destructible = registry.get_component<AutoDestructible>(entity);
        if (destructible->lifeTime > 0) {
            destructible->lifeTime -= deltaTime;
            if (destructible->lifeTime <= 0) {
                std::cout << "Entity " << entity << " destroyed" << std::endl; //TODO: remove
                registry.kill_entity(entity);
                continue;
            }
        }
        // if (destructible->killCondition()) {
        //     registry.kill_entity(entity);
        // }
    }
}
} // namespace RealEngine