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
    auto* colliderScore = registry.get_component<RealEngine::Score>(collider);
    auto* container     = registry.get_component<RealEngine::NetvarContainer>(entity);
    auto  playerID      = std::any_cast<size_t>(container->getNetvar("playerID")->value);

    if (container && colliderScore) {
        std::shared_ptr<RealEngine::Entity> player = registry.entity_from_index(playerID);
        if (player) {
            auto* playerScore = registry.get_component<RealEngine::Score>(*player);
            if (playerScore) {
                playerScore->toAdd += colliderScore->amount;
            }
        }
    }
}

static void bulletTakesDamage(RealEngine::CollisionType collisionType,
                              RealEngine::Registry& registry, RealEngine::Entity collider,
                              RealEngine::Entity entity) {
    auto* health         = registry.get_component<RealEngine::Health>(entity);
    auto* colliderHealth = registry.get_component<RealEngine::Health>(collider);
    auto* damage         = registry.get_component<RealEngine::Damage>(entity);

    if ((colliderHealth && damage) && (colliderHealth->amount >= damage->amount)) {
        selfDestruct(registry, entity);
        addScoreToPlayer(registry, entity, collider);
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
    : _entity(registry.spawn_entity()) {
    registry.add_component(_entity, RealEngine::Position{position.x, position.y});
    registry.add_component(_entity, RealEngine::Velocity{speed, 0, {speed, speed}, 0.f});
    auto spriteSheet = RealEngine::AssetManager::getInstance().getSpriteSheet(spriteName);
    if (spriteSheet) {
        registry.add_component(_entity, RealEngine::SpriteSheet{*spriteSheet});
    } else {
        auto sprite = RealEngine::AssetManager::getInstance().getSprite(spriteName);
        if (sprite) {
            registry.add_component(_entity, RealEngine::SpriteComponent{*sprite});
        } else {
            std::cerr << "Failed to load Sprite or SpriteSheet for ID: " << spriteName << std::endl;
        }
    }
    registry.add_component(_entity, RealEngine::Drawable{});
    registry.add_component(
        _entity,
        RealEngine::Collision{{-1.f, -1.f, -1.f, -1.f},  // automatic adjustement of collision
                              "bullet",
                              false,
                              RealEngine::CollisionType::ALLY_BULLET,
                              bulletHandleCollision});
    registry.add_component(_entity, RealEngine::AutoDestructible{5});
    registry.add_component(_entity, RealEngine::Damage{damage});
    registry.add_component(_entity, RealEngine::Health{health, health});
    registry.add_component(_entity, RealEngine::NetvarContainer{{
                                        {"playerID", {"size_t", "playerID", playerID, nullptr}},
                                    }});
}

Bullet::~Bullet() {}
}  // namespace rtype