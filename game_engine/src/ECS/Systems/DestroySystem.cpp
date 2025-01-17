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
        // registry.remove_entity(entity);
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

// auto* colliderScore = registry.get_component<RealEngine::Score>(collider);
// auto* container     = registry.get_component<RealEngine::NetvarContainer>(entity);
// if (!container) return;
// auto playerID = std::any_cast<size_t>(container->getNetvar("playerID")->value);
// auto player   = registry.entity_from_index(playerID);
// if (!player) return;
// auto playerScore = registry.get_component<RealEngine::Score>(*player);
// auto playerNetvarContainer = registry.get_component<RealEngine::NetvarContainer>(*player);
// if (playerScore && playerNetvarContainer) {
//     // std::cout << "Player get score amount:" << colliderScore->amount << std::endl;
//     playerScore->amount += colliderScore->amount;
//     auto score_health_update = playerNetvarContainer->getNetvar("score_health_update");
//     if (score_health_update) {
//         score_health_update->value = true;
//     }
// }

bool DestroySystem::healthDestroy(Registry& registry, Entity entity) {
    auto* health = registry.get_component<Health>(entity);

    if (!health || health->amount > 0) return false;

    if (registry.is_valid(entity)) {
        killEntity(registry, entity);
    }

    std::shared_ptr<Entity> playerBullet = registry.entity_from_index(health->lastDamager);
    if (!playerBullet) return true;

    auto* colliderScore = registry.get_component<Score>(entity);
    auto* container     = registry.get_component<NetvarContainer>(playerBullet);
    if (!container || !colliderScore) return true;

    auto playerID = std::any_cast<size_t>(container->getNetvar("playerID")->value);
    auto player   = registry.entity_from_index(playerID);
    if (!player) return true;

    auto playerScore           = registry.get_component<Score>(player);
    auto playerNetvarContainer = registry.get_component<NetvarContainer>(player);
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