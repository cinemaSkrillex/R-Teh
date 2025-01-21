/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** Bullet
*/

#include "Game/Player/Bullet.hpp"

namespace rtype {

static void addScoreToPlayer(RealEngine::Registry& registry, RealEngine::Entity entity,
                             RealEngine::Entity collider) {
    std::cout << "Add score to player" << std::endl;
    auto* colliderScore = registry.getComponent<RealEngine::Score>(collider);
    auto* container     = registry.getComponent<RealEngine::NetvarContainer>(entity);
    if (!container) return;
    auto playerID = std::any_cast<size_t>(container->getNetvar("playerID")->value);
    auto player   = registry.entityFromIndex(playerID);
    if (!player) return;
    auto playerScore           = registry.getComponent<RealEngine::Score>(*player);
    auto playerNetvarContainer = registry.getComponent<RealEngine::NetvarContainer>(*player);
    if (playerScore && playerNetvarContainer) {
        // std::cout << "Player get score amount:" << colliderScore->amount << std::endl;
        playerScore->amount += colliderScore->amount;
        auto score_health_update = playerNetvarContainer->getNetvar("score_health_update");
        if (score_health_update) {
            score_health_update->value = true;
        }
    }
}

static void bulletTakesDamage(RealEngine::CollisionType collisionType,
                              RealEngine::Registry& registry, RealEngine::Entity collider,
                              RealEngine::Entity entity) {
    auto* health         = registry.getComponent<RealEngine::Health>(entity);
    auto* colliderHealth = registry.getComponent<RealEngine::Health>(collider);
    auto* damage         = registry.getComponent<RealEngine::Damage>(entity);

    if ((colliderHealth && damage) && (colliderHealth->amount >= damage->amount)) {
        selfDestruct(registry, entity);
        // addScoreToPlayer(registry, entity, collider);
        // } else {
        //     health->amount -= damage->amount;
        //     if (health->amount <= 0) addScoreToPlayer(registry, entity, collider);
    }
}

static void bulletHandleCollision(RealEngine::CollisionType collisionType,
                                  RealEngine::Registry& registry, RealEngine::Entity collider,
                                  RealEngine::Entity entity) {
    switch (collisionType) {
        case RealEngine::CollisionType::INACTIVE:
            break;
        case RealEngine::CollisionType::SOLID:
            selfDestruct(registry, entity);
            break;
        case RealEngine::CollisionType::PICKABLE:
            break;
        case RealEngine::CollisionType::OTHER:
            break;
        case RealEngine::CollisionType::PLAYER:
            break;
        case RealEngine::CollisionType::ENEMY:
            // selfDestruct(registry, entity);
            bulletTakesDamage(collisionType, registry, collider, entity);
            break;
        case RealEngine::CollisionType::ALLY_BULLET:
            break;
        case RealEngine::CollisionType::ENEMY_BULLET:
            break;
        default:
            break;
    }
}

Bullet::Bullet(RealEngine::Registry& registry, sf::Vector2f position, float speed,
               std::string spriteName, float damage, int health, size_t playerID)
    : _entity(registry.spawnEntity()) {
    registry.addComponent(_entity, RealEngine::Position{position.x, position.y});
    registry.addComponent(_entity, RealEngine::Velocity{speed, 0, {speed, speed}, 0.f});
    auto spriteSheet = RealEngine::AssetManager::getInstance().getSpriteSheet(spriteName);
    if (spriteSheet) {
        registry.addComponent(_entity, RealEngine::SpriteSheet{*spriteSheet});
    } else {
        auto sprite = RealEngine::AssetManager::getInstance().getSprite(spriteName);
        if (sprite) {
            registry.addComponent(_entity, RealEngine::SpriteComponent{*sprite});
        } else {
            std::cerr << "Failed to load Sprite or SpriteSheet for ID: " << spriteName << std::endl;
        }
    }
    registry.addComponent(_entity, RealEngine::Drawable{});
    registry.addComponent(
        _entity,
        RealEngine::Collision{{-1.f, -1.f, -1.f, -1.f},  // automatic adjustement of collision
                              "bullet",
                              false,
                              RealEngine::CollisionType::ALLY_BULLET,
                              bulletHandleCollision});
    registry.addComponent(_entity, RealEngine::AutoDestructible{5});
    registry.addComponent(_entity, RealEngine::Damage{damage});
    registry.addComponent(_entity, RealEngine::Health{health, health});
    registry.addComponent(_entity, RealEngine::NetvarContainer{{
                                       {"playerID", {"size_t", "playerID", playerID, nullptr}},
                                   }});
}

Bullet::~Bullet() {}
}  // namespace rtype