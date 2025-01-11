/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** TargetRadiusSystem
*/

#include "../include/ECS/Systems/TargetRadiusSystem.hpp"

namespace RealEngine {

void TargetRadiusSystem::update(Registry& registry) {
    auto                radius_entities = registry.view<Position, TargetRadius>();
    auto                player_entities = registry.view<Position, Controllable>();
    std::vector<Entity> players_targets;

    if (radius_entities.empty() || player_entities.empty()) {
        return;
    }
    for (auto entity : radius_entities) {
        auto* entity_position = registry.get_component<Position>(entity);
        auto* entity_radius   = registry.get_component<TargetRadius>(entity);
        auto* entity_target   = registry.get_component<Target>(entity);

        if (entity_target && entity_target->target) continue;
        players_targets.clear();
        for (auto player : player_entities) {
            auto* player_position = registry.get_component<Position>(player);
            float distance        = sqrt(pow(player_position->x - entity_position->x, 2) +
                                         pow(player_position->y - entity_position->y, 2));
            if (distance < entity_radius->size) {
                players_targets.push_back(player);
            }
        }
        if (players_targets.size() == 1) {
            registry.add_component(entity, Target{std::make_shared<Entity>(players_targets[0])});
        } else if (!players_targets.empty()) {
            std::srand(std::time(nullptr));
            int random_index = std::rand() % players_targets.size();
            registry.add_component(entity,
                                   Target{std::make_shared<Entity>(players_targets[random_index])});
        }
    }
}

}  // namespace RealEngine