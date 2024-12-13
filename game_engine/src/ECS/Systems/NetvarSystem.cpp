/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** NetvarSystem
*/

#include "ECS/Systems/NetvarSystem.hpp"

namespace RealEngine {
void NetvarSystem::update(Registry& registry, float deltaTime) {
    auto entities = registry.view<Netvar>();

    if (entities.empty()) {
        return;
    }
    for (auto entity : entities) {
        auto netvar = registry.get_component<Netvar>(entity);
        if (netvar->updateCallback != nullptr) {
            netvar->updateCallback(registry, entity, deltaTime);
        }
    }
}
}  // namespace RealEngine