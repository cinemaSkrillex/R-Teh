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
    registry.add_component(_playerEntity, RealEngine::Health{50, 200});
    registry.add_component(_playerEntity,
                           RealEngine::Netvar{"PLAYER", "shootCooldown", 0.5f, updateCooldown});
    registry.add_component(
        _playerEntity,
        RealEngine::SpriteSheet{
            _spaceshipSheet, "idle", 0, {32, 15}, false, false, 100, {-1, -1}, sf::Clock()});
    registry.add_component(
        _playerEntity,
        RealEngine::Collision{
            {0.0f, 0.0f, 32.f * GAME_SCALE, 15.f * GAME_SCALE},
            "spaceship",
            false,
            RealEngine::CollisionType::PLAYER,
            [this](RealEngine::CollisionType collisionType, RealEngine::Registry& registry,
                   RealEngine::Entity collider, RealEngine::Entity entity) {
                player_collision_handler(collisionType, registry, collider, entity);
            }});
}

Player::~Player() {}

void Player::player_collision_handler(RealEngine::CollisionType collisionType,
                                      RealEngine::Registry& registry, RealEngine::Entity collider,
                                      RealEngine::Entity entity) {
    switch (collisionType) {
        case RealEngine::CollisionType::INACTIVE:
            break;
        case RealEngine::CollisionType::SOLID:
            player_collide_with_ground(registry, entity);
            break;
        // case RealEngine::CollisionType::HIT:
        //     player_take_damage(registry, collider);
        //     break;
        case RealEngine::CollisionType::PICKABLE:
            // handle pickable
            break;
        case RealEngine::CollisionType::OTHER:
            // std::cout << "OTHER" << std::endl;
            break;
        case RealEngine::CollisionType::ENEMY:
            break;
        case RealEngine::CollisionType::ALLY_BULLET:
            break;
        case RealEngine::CollisionType::ENEMY_BULLET:
            player_take_damage(registry, collider, entity);
            break;
        default:
            break;
    }
}

void Player::player_collide_with_ground(RealEngine::Registry& registry, RealEngine::Entity entity) {
    // auto* playerPosition = registry.get_component<RealEngine::Position>(*_playerEntity);
    // auto* playerVelocity = registry.get_component<RealEngine::Velocity>(*_playerEntity);
    auto* playerPosition = registry.get_component<RealEngine::Position>(entity);
    auto* playerVelocity = registry.get_component<RealEngine::Velocity>(entity);

    playerVelocity->vy = 0;
    playerVelocity->vx = 0;
    playerPosition->y -= 1;
    playerPosition->x -= 1;
}

void Player::player_take_damage(RealEngine::Registry& registry, RealEngine::Entity collider,
                                RealEngine::Entity entity) {
    auto* playerHealth   = registry.get_component<RealEngine::Health>(entity);
    auto* colliderDamage = registry.get_component<RealEngine::Damage>(collider);

    if (playerHealth) {
        if (colliderDamage) {
            std::cerr << "ENEMY COLLIDED" << std::endl;
            playerHealth->damage += colliderDamage->amount;
        }
    }
}
}  // namespace rtype