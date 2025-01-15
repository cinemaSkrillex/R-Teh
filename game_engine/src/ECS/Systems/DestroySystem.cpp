/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** DestroySystem
*/

#include "../include/ECS/Systems/DestroySystem.hpp"

#include <iostream>

namespace RealEngine {

DestroySystem::DestroySystem() {}

void DestroySystem::killEntity(Registry& registry, Entity entity) {
    if (registry.is_valid(entity)) {
        _deadEntities.push_back(entity);
    }
}

bool DestroySystem::autoDestroy(Registry& registry, Entity entity, float deltaTime) {
    auto* destructible = registry.get_component<AutoDestructible>(entity);

    if (destructible) {
        if (destructible->lifeTime >= 0) {
            destructible->lifeTime -= deltaTime;
            if (destructible->lifeTime <= 0) {
                if (registry.is_valid(entity)) {
                    killEntity(registry, entity);
                    return true;
                }
            }
        }
        if (destructible->kill) {
            if (registry.is_valid(entity)) {
                killEntity(registry, entity);
                return true;
            }
        }
        if (destructible->killCondition) {
            destructible->kill = true;
        }
    }
    return false;
}

bool DestroySystem::healthDestroy(Registry& registry, Entity entity) {
    auto* health = registry.get_component<Health>(entity);

    if (health) {
        if (health->amount <= 0) {
            if (registry.is_valid(entity)) {
                killEntity(registry, entity);
                return true;
            }
        }
    }
    return false;
}

void DestroySystem::update(Registry& registry, float deltaTime) {
    auto entities = registry.view<>();

    for (auto& entity : _deadEntities) {
        registry.remove_entity(entity);
    }
    _deadEntities.clear();
    if (entities.empty()) {
        return;
    }
    for (auto entity : entities) {
        if (autoDestroy(registry, entity, deltaTime)) continue;
        if (healthDestroy(registry, entity)) continue;
    }
}

}  // namespace RealEngine