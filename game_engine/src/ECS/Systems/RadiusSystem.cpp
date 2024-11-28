#include "../include/ECS/Systems/RadiusSystem.hpp"

namespace RealEngine {

RadiusSystem::RadiusSystem() {}

RadiusSystem::~RadiusSystem() {}

void RadiusSystem::update(Registry& registry, float deltaTime) {
    // auto entities = registry.view<Position, Radius>();

    // for (auto entity : entities) {
    //     auto* position = registry.get_component<Position>(entity);
    //     auto* radius   = registry.get_component<Radius>(entity);

    //     if (position && radius) {
    //         radius->behavior(registry, entity, deltaTime);
    //     }
    // }
    return;
}

}  // namespace RealEngine