#include "Game/Player/Player.hpp"

namespace rtype {

static void playerTakeDamage(RealEngine::Registry& registry, RealEngine::Entity collider,
                             RealEngine::Entity entity) {
    auto* playerHealth   = registry.get_component<RealEngine::Health>(entity);
    auto* colliderDamage = registry.get_component<RealEngine::Damage>(collider);

    if (playerHealth && colliderDamage && playerHealth->invincibilityTime <= 0.0f) {
        auto* playerPosition = registry.get_component<RealEngine::Position>(entity);
        std::cout << "Player take damage amount:" << colliderDamage->amount << std::endl;
        if (colliderDamage->effect) {
            playerHealth->regenerationRate     = -colliderDamage->amount;
            playerHealth->regenerationCooldown = colliderDamage->effectInterval;
            playerHealth->regenerationTimer    = 0.0f;
            playerHealth->regenerationTime     = colliderDamage->effectDuration;
        } else {
            playerHealth->amount -= colliderDamage->amount;
        }
        playerHealth->invincibilityTime = 1.5f;
    }
}

static void playerCollisionHandler(RealEngine::CollisionType collisionType,
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
        case RealEngine::CollisionType::ENEMY:
            playerTakeDamage(registry, collider, entity);
            break;
        case RealEngine::CollisionType::ALLY_BULLET:
            break;
        case RealEngine::CollisionType::ENEMY_BULLET:
            playerTakeDamage(registry, collider, entity);
            break;
        case RealEngine::CollisionType::BLOCKING:
            blockEntity(registry, collider, entity);
            break;
        default:
            break;
    }
}

static void updateCooldown(RealEngine::Registry& registry, RealEngine::Entity& entity,
                           RealEngine::Netvar& currentNetvar, float deltaTime) {
    currentNetvar.value = std::any_cast<float>(currentNetvar.value) - deltaTime;
    if (std::any_cast<float>(currentNetvar.value) < 0) {
        currentNetvar.value = 0.f;
    }
}

static void updateHoldTime(RealEngine::Registry& registry, RealEngine::Entity& entity,
                           RealEngine::Netvar& currentNetvar, float deltaTime) {
    auto* container = registry.get_component<RealEngine::NetvarContainer>(entity);
    auto* isHolding = container->getNetvar("isHolding");
    if (std::any_cast<bool>(isHolding->value)) {
        currentNetvar.value = std::any_cast<float>(currentNetvar.value) + deltaTime;
    } else {
        currentNetvar.value = 0.f;
    }
}

static void updateInvincibilityAnim(RealEngine::Registry& registry, RealEngine::Entity& entity,
                                    RealEngine::Netvar& currentNetvar, float deltaTime) {
    currentNetvar.value = std::any_cast<float>(currentNetvar.value) - deltaTime;
    auto* health        = registry.get_component<RealEngine::Health>(entity);
    auto* spritesheet   = registry.get_component<RealEngine::SpriteSheet>(entity);

    if (!health || !spritesheet) {
        return;
    }
    if (health->invincibilityTime > 0.0f) {
        if (std::any_cast<float>(currentNetvar.value) < 0) {
            for (auto& [key, sprite] : spritesheet->sprites) {
                if (sprite.getOpacity() == 255) {
                    sprite.setOpacity(0);
                } else {
                    sprite.setOpacity(255);
                }
            }
            currentNetvar.value = 0.25f;
        }
    } else {
        for (auto& [key, sprite] : spritesheet->sprites) {
            if (sprite.getOpacity() != 255) {
                sprite.setOpacity(255);
            }
        }
        currentNetvar.value = 0.25f;
    }
}

PlayerEntity::PlayerEntity(RealEngine::Registry& registry, sf::Vector2f position)
    : _entity(registry.spawn_entity()) {
    _playerSpriteSheet.emplace(
        "idle", *(RealEngine::AssetManager::getInstance().getSprite("spaceship_idle")));
    _playerSpriteSheet.emplace(
        "up", *(RealEngine::AssetManager::getInstance().getSprite("spaceship_up")));
    _playerSpriteSheet.emplace(
        "down", *(RealEngine::AssetManager::getInstance().getSprite("spaceship_down")));
    registry.add_component(_entity, RealEngine::Position{position.x, position.y});
    registry.add_component(_entity, RealEngine::Velocity{0.0f, 0.0f, {300.0f, 300.0f}, 3.0f});
    registry.add_component(_entity, RealEngine::Acceleration{1000.0f, 1000.0f, 1000.0f});
    registry.add_component(_entity, RealEngine::Controllable{});
    registry.add_component(_entity, RealEngine::Health{100, 200});
    registry.add_component(_entity,
                           RealEngine::NetvarContainer{{
                               {"shootCooldown", {"float", "shootCooldown", 0.5f, updateCooldown}},
                               {"holdTime", {"float", "holdTime", 0.0f, updateHoldTime}},
                               {"isHolding", {"bool", "isHolding", false, nullptr}},
                               {"invincibilityAnimTimer",
                                {"float", "invincibilityAnimTimer", 0.0f, updateInvincibilityAnim}},
                           }});
    registry.add_component(
        _entity,
        RealEngine::SpriteSheet{
            _playerSpriteSheet, "idle", 0, {32, 15}, false, false, 100, {-1, -1}, sf::Clock()});
    registry.add_component(_entity,
                           RealEngine::Collision{{0.f, 0.f, 32.f * GAME_SCALE, 15.f * GAME_SCALE},
                                                 "spaceship",
                                                 false,
                                                 RealEngine::CollisionType::PLAYER,
                                                 playerCollisionHandler});
}

PlayerEntity::~PlayerEntity() {}

}  // namespace rtype