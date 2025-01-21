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
    if (registry.isValid(entity)) {
        _deadEntities.push_back(entity);
        // registry.removeEntity(entity);
    }
}

bool DestroySystem::autoDestroy(Registry& registry, Entity entity, float deltaTime) {
    auto* destructible = registry.getComponent<AutoDestructible>(entity);

    if (destructible) {
        if (destructible->lifeTime >= 0) {
            destructible->lifeTime -= deltaTime;
            if (destructible->lifeTime <= 0) {
                if (registry.isValid(entity)) {
                    killEntity(registry, entity);
                    return true;
                }
            }
        }
        if (destructible->kill) {
            if (registry.isValid(entity)) {
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
    auto* health = registry.getComponent<Health>(entity);

    if (!health || health->amount > 0) return false;

    if (registry.isValid(entity)) {
        killEntity(registry, entity);
    }

    std::shared_ptr<Entity> playerBullet = registry.entityFromIndex(health->lastDamager);
    if (!playerBullet) return true;

    auto* colliderScore = registry.getComponent<Score>(entity);
    auto* container     = registry.getComponent<NetvarContainer>(playerBullet);
    if (!container || !colliderScore) return true;

    auto playerID = std::any_cast<size_t>(container->getNetvar("playerID")->value);
    auto player   = registry.entityFromIndex(playerID);
    if (!player) return true;

    auto playerScore           = registry.getComponent<Score>(player);
    auto playerNetvarContainer = registry.getComponent<NetvarContainer>(player);
    if (!playerScore || !playerNetvarContainer) return true;

    playerScore->amount += colliderScore->amount;
    auto score_health_update = playerNetvarContainer->getNetvar("score_health_update");
    if (score_health_update) {
        score_health_update->value = true;
    }

    return true;
}

void DestroySystem::update(Registry& registry, float deltaTime) {
    auto entities = registry.view<>();

    for (auto& entity : _deadEntities) {
        if (registry.isValid(entity)) registry.killEntity(entity);
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