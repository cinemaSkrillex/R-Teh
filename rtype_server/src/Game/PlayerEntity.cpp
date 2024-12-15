/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** PlayerEntity
*/

#include "../include/Game/PlayerEntity.hpp"

#include "../include/ECS/Registry/Registry.hpp"

static void updateCooldown(RealEngine::Registry& registry, RealEngine::Entity& entity,
                           float deltaTime) {
    std::vector<RealEngine::Netvar*> netvars = registry.get_components<RealEngine::Netvar>(entity);
    for (auto& netvar : netvars) {
        if (netvar->name != "shootCooldown") continue;
        netvar->value = std::any_cast<float>(netvar->value) - deltaTime;
        if (std::any_cast<float>(netvar->value) < 0) {
            netvar->value = 0.f;
        }
    }
}

namespace rtype {
Player::Player(RealEngine::Registry& registry, sf::Vector2f position,
               std::unordered_map<std::string, RealEngine::Sprite> playerSprites)
    : _playerEntity(registry.spawn_entity()), _spaceshipSheet(playerSprites) {
    registry.add_component(_playerEntity, RealEngine::Position{position.x, position.y});
    registry.add_component(_playerEntity, RealEngine::Velocity{0.0f, 0.0f, {300.0f, 300.0f}, 3.0f});
    registry.add_component(_playerEntity, RealEngine::Acceleration{1000.0f, 1000.0f, 1000.0f});
    registry.add_component(_playerEntity, RealEngine::Controllable{});
    registry.add_component(_playerEntity, RealEngine::Health{100, 200});
    registry.add_component(_playerEntity,
                           RealEngine::Netvar{"PLAYER", "shootCooldown", 0.f, updateCooldown});
    registry.add_component(
        _playerEntity,
        RealEngine::SpriteSheet{_spaceshipSheet, "idle", 0, {32, 15}, false, false, 100});
    registry.add_component(
        _playerEntity,
        RealEngine::Collision{{0.0f, 0.0f, 32.f * GAME_SCALE, 15.f * GAME_SCALE},
                              "spaceship",
                              false,
                              RealEngine::CollisionType::OTHER,
                              [this](RealEngine::CollisionType collisionType,
                                     RealEngine::Registry& registry, RealEngine::Entity collider) {
                                  player_collision_handler(collisionType, registry, collider);
                              }});
}

Player::~Player() {}

void Player::player_collision_handler(RealEngine::CollisionType collisionType,
                                      RealEngine::Registry& registry, RealEngine::Entity collider) {
    std::cout << "Player collided" << std::endl;
    switch (collisionType) {
        case RealEngine::CollisionType::INACTIVE:
            return;
            break;
        case RealEngine::CollisionType::SOLID:
            player_collide_with_ground(registry);
            break;
        case RealEngine::CollisionType::HIT:
            player_take_damage(registry, collider);
            break;
        case RealEngine::CollisionType::PICKABLE:
            return;
            break;
        default:
            break;
    }
}

void Player::player_collide_with_ground(RealEngine::Registry& registry) {
    auto* playerPosition = registry.get_component<RealEngine::Position>(*_playerEntity);
    auto* playerVelocity = registry.get_component<RealEngine::Velocity>(*_playerEntity);

    playerVelocity->vy = 0;
    playerVelocity->vx = 0;
    playerPosition->y -= 1;
    playerPosition->x -= 1;
}

void Player::player_take_damage(RealEngine::Registry& registry, RealEngine::Entity collider) {
    auto* playerHealth   = registry.get_component<RealEngine::Health>(*_playerEntity);
    auto* colliderDamage = registry.get_component<RealEngine::Damage>(collider);

    if (playerHealth) {
        std::cout << "Player took damage" << std::endl;
        if (colliderDamage) {
            playerHealth->damage += colliderDamage->amount;
            std::cout << "Player took " << colliderDamage->amount << " damage" << std::endl;
        }
    }
}
}  // namespace rtype