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
        for (auto& component : registry.get_components<Netvar>(entity)) {
            if (component->updateCallback != nullptr) {
                component->updateCallback(registry, entity, deltaTime);
            }
        }
    }
}
}  // namespace RealEngine