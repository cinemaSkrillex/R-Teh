#include "Game/Player/Player.hpp"

#include "Game/Particles.hpp"

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
            playerHealth->damage += colliderDamage->amount;
        }
        playerHealth->invincibilityTime = 1.5f;
        HitEffect hitEffect(registry, {playerPosition->x, playerPosition->y});
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
        default:
            break;
    }
}

Player::Player(RealEngine::Registry& registry, sf::Vector2f position, bool otherPlayer)
    : _entity(registry.spawn_entity()) {
    if (!otherPlayer) {
        _playerSpriteSheet.emplace(
            "idle", *(RealEngine::AssetManager::getInstance().getSprite("spaceship_idle")));
        _playerSpriteSheet.emplace(
            "up", *(RealEngine::AssetManager::getInstance().getSprite("spaceship_up")));
        _playerSpriteSheet.emplace(
            "down", *(RealEngine::AssetManager::getInstance().getSprite("spaceship_down")));
        registry.add_component(_entity, RealEngine::Position{200.f, 200.f});
        registry.add_component(_entity, RealEngine::Velocity{0.0f, 0.0f, {300.0f, 300.0f}, 3.0f});
        registry.add_component(_entity, RealEngine::Acceleration{10.0f, 10.0f, 10.0f});
        registry.add_component(_entity, RealEngine::Controllable{});
        registry.add_component(_entity, RealEngine::Drawable{});
        registry.add_component(
            _entity,
            RealEngine::SpriteSheet{
                _playerSpriteSheet, "idle", 0, {32, 15}, false, false, 100, {-1, -1}, sf::Clock()});
        registry.add_component(
            _entity, RealEngine::Collision{{0.f, 0.f, 32.f * GAME_SCALE, 15.f * GAME_SCALE},
                                           "spaceship",
                                           false,
                                           RealEngine::CollisionType::OTHER,
                                           playerCollisionHandler});
        registry.add_component(_entity, RealEngine::Health{100, 200});
    } else {
        registry.add_component(_entity, RealEngine::Position{position.x, position.y});
        registry.add_component(_entity, RealEngine::Drawable{});
        registry.add_component(
            _entity, RealEngine::Interpolation{
                         {position.x, position.y}, {position.x, position.y}, 0.f, 1.f, false});
        registry.add_component(
            _entity, RealEngine::SpriteComponent{*(
                         (RealEngine::AssetManager::getInstance().getSprite("spaceship_other")))});
    }
}

Player::~Player() {}

}  // namespace rtype