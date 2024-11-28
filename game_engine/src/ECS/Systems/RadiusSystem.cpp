#include "../include/ECS/Systems/RadiusSystem.hpp"

namespace RealEngine {

RadiusSystem::RadiusSystem() {}

RadiusSystem::~RadiusSystem() {}

void RadiusSystem::update(Registry& registry) {
    auto radius_entities = registry.view<Position, Radius>();
    auto player_entities = registry.view<Position, Controllable>();
    bool in, outer_max, outer_min = false;

    for (auto entity : radius_entities) {
        auto* entity_position = registry.get_component<Position>(entity);
        auto* entity_radius   = registry.get_component<Radius>(entity);
        in                    = false;
        outer_max             = false;
        outer_min             = false;

        for (auto player : player_entities) {
            auto* player_position = registry.get_component<Position>(player);
            float distance        = sqrt(pow(player_position->x - entity_position->x, 2) +
                                         pow(player_position->y - entity_position->y, 2));
            if (distance < entity_radius->sizeMin) {
                outer_min = true;
            } else if (distance < entity_radius->sizeMax) {
                in = true;
            } else if (distance > entity_radius->sizeMax) {
                outer_max = true;
            }
        }
        if (outer_min && entity_radius->lastState != RadiusState::OUTER_MIN) {
            entity_radius->behaviorOuterMin();
            entity_radius->lastState = RadiusState::OUTER_MIN;
        } else if (in && entity_radius->lastState != RadiusState::IN) {
            entity_radius->behaviorIn();
            entity_radius->lastState = RadiusState::IN;
        } else if (outer_max && entity_radius->lastState != RadiusState::OUTER_MAX) {
            entity_radius->behaviorOuterMax();
            entity_radius->lastState = RadiusState::OUTER_MAX;
        }
    }
    return;
}

void setBehaviorIn(Entity entity, Registry& registry, std::function<void()> behavior) {
    auto* radius = registry.get_component<Radius>(entity);
    if (radius) {
        radius->behaviorIn = behavior;
    } else {
        std::cerr << "Error: Entity does not have a Radius component!" << std::endl;
    }
}

void setBehaviorOuterMax(Entity entity, Registry& registry, std::function<void()> behavior) {
    auto* radius = registry.get_component<Radius>(entity);
    if (radius) {
        radius->behaviorOuterMax = behavior;
    } else {
        std::cerr << "Error: Entity does not have a Radius component!" << std::endl;
    }
}

void setBehaviorOuterMin(Entity entity, Registry& registry, std::function<void()> behavior) {
    auto* radius = registry.get_component<Radius>(entity);
    if (radius) {
        radius->behaviorOuterMin = behavior;
    } else {
        std::cerr << "Error: Entity does not have a Radius component!" << std::endl;
    }
}
}  // namespace RealEngine