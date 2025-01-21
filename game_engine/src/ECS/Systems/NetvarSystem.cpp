/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** NetvarSystem
*/

#include "ECS/Systems/NetvarSystem.hpp"

namespace RealEngine {
void NetvarSystem::update(Registry& registry, float deltaTime) {
    updateNetvars(registry, deltaTime);
    updateNetvarsContainers(registry, deltaTime);
}

void NetvarSystem::updateNetvars(Registry& registry, float deltaTime) {
    auto entities = registry.view<Netvar>();

    if (entities.empty()) {
        return;
    }
    for (auto entity : entities) {
        for (auto& component : registry.getComponents<Netvar>(entity)) {
            if (component->updateCallback != nullptr) {
                component->updateCallback(registry, entity, *component, deltaTime);
            }
        }
    }
}

void NetvarSystem::updateNetvarsContainers(Registry& registry, float deltaTime) {
    auto entities = registry.view<NetvarContainer>();

    if (entities.empty()) {
        return;
    }
    for (auto entity : entities) {
        for (auto& component : registry.getComponents<NetvarContainer>(entity)) {
            for (auto& [key, value] : component->netvars) {
                if (value.updateCallback != nullptr) {
                    value.updateCallback(registry, entity, value, deltaTime);
                }
            }
        }
    }
}
}  // namespace RealEngine