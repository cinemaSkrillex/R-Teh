#include "Game/Player/Player.hpp"

namespace rtype {

static void selfDestruct(RealEngine::Registry& registry, RealEngine::Entity entity) {
    auto* health = registry.getComponent<RealEngine::Health>(entity);

    if (health) {
        health->amount -= health->maxHealth;
    }
}

static void playerTakeDamage(RealEngine::Registry& registry, RealEngine::Entity collider,
                             RealEngine::Entity entity) {
    auto* playerHealth   = registry.getComponent<RealEngine::Health>(entity);
    auto* colliderDamage = registry.getComponent<RealEngine::Damage>(collider);

    if (playerHealth && colliderDamage && playerHealth->invincibilityTime <= 0.0f) {
        auto* playerPosition = registry.getComponent<RealEngine::Position>(entity);
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

static void updateClignotingAnim(RealEngine::Registry& registry, RealEngine::Entity& entity,
                                 RealEngine::Netvar& currentNetvar, float deltaTime) {
    currentNetvar.value = std::any_cast<float>(currentNetvar.value) - deltaTime;
    auto* health        = registry.getComponent<RealEngine::Health>(entity);
    auto* spritesheet   = registry.getComponent<RealEngine::SpriteSheet>(entity);
    auto* netvars       = registry.getComponent<RealEngine::NetvarContainer>(entity);

    if (!health || !spritesheet || !netvars) {
        return;
    }
    auto* timerNetvar = netvars->getNetvar("clignotingFrameTimer");
    float frameTimer  = std::any_cast<float>(timerNetvar->value);
    if (std::any_cast<float>(currentNetvar.value) > 0) {
        frameTimer -= deltaTime;
        if (frameTimer <= 0.f) {
            for (auto& [key, sprite] : spritesheet->sprites) {
                if (sprite.getOpacity() == 255) {
                    sprite.setOpacity(50);
                } else {
                    sprite.setOpacity(255);
                }
            }
            frameTimer = 0.25f;
        }
        timerNetvar->value = frameTimer;

    } else {
        if (timerNetvar) timerNetvar->value = 0.25f;
        for (auto& [key, sprite] : spritesheet->sprites) {
            if (sprite.getOpacity() != 255) {
                sprite.setOpacity(255);
            }
        }
    }
}

static void blockPlayer(RealEngine::Registry& registry, RealEngine::Entity collider,
                        RealEngine::Entity entity) {
    auto* playerCollision   = registry.getComponent<RealEngine::Collision>(entity);
    auto* colliderCollision = registry.getComponent<RealEngine::Collision>(collider);
    auto* playerPosition    = registry.getComponent<RealEngine::Position>(entity);

    if (playerCollision && colliderCollision) {
        sf::FloatRect playerBounds   = playerCollision->bounds;
        sf::FloatRect colliderBounds = colliderCollision->bounds;

        float overlapLeft   = (playerBounds.left + playerBounds.width) - colliderBounds.left;
        float overlapRight  = (colliderBounds.left + colliderBounds.width) - playerBounds.left;
        float overlapTop    = (playerBounds.top + playerBounds.height) - colliderBounds.top;
        float overlapBottom = (colliderBounds.top + colliderBounds.height) - playerBounds.top;

        float correctionX = (overlapLeft < overlapRight) ? -overlapLeft : overlapRight;
        float correctionY = (overlapTop < overlapBottom) ? -overlapTop : overlapBottom;

        if (std::abs(correctionX) < std::abs(correctionY)) {
            playerPosition->x += correctionX;
        } else {
            playerPosition->y += correctionY;
        }
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
            blockPlayer(registry, collider, entity);
        default:
            break;
    }
}

Player::Player(RealEngine::Registry& registry, sf::Vector2f position, bool otherPlayer)
    : _entity(registry.spawnEntity()) {
    if (!otherPlayer) {
        registry.addComponent(_entity, RealEngine::Position{200.f, 200.f});
        registry.addComponent(_entity, RealEngine::Velocity{0.0f, 0.0f, {300.0f, 300.0f}, 3.0f});
        registry.addComponent(_entity, RealEngine::Acceleration{1000.0f, 1000.0f, 1000.0f});
        registry.addComponent(_entity, RealEngine::Controllable{});
        registry.addComponent(_entity, RealEngine::Drawable{});
        auto spriteSheet = RealEngine::AssetManager::getInstance().getSpriteSheet("spaceship");
        if (spriteSheet) {
            registry.addComponent(_entity, RealEngine::SpriteSheet{*spriteSheet});
        } else {
            std::cerr << "Failed to load Sprite or SpriteSheet for player" << std::endl;
        }
        registry.addComponent(
            _entity, RealEngine::Collision{{0.f, 0.f, 32.f * GAME_SCALE, 15.f * GAME_SCALE},
                                           "spaceship",
                                           false,
                                           RealEngine::CollisionType::OTHER,
                                           playerCollisionHandler});
        registry.addComponent(_entity, RealEngine::Health{100, 200, *_entity});
        registry.addComponent(_entity, RealEngine::Score{0});
        registry.addComponent(
            _entity,
            RealEngine::NetvarContainer{{
                {"clignotingTimer", {"float", "clignotingTimer", float(0.f), updateClignotingAnim}},
                {"clignotingFrameTimer", {"float", "clignotingFrameTimer", float(0.25f), nullptr}},
                {"hold_shoot", {"float", "hold_shoot", float(0.f), nullptr}},
            }});
    } else {
        registry.addComponent(_entity, RealEngine::Position{position.x, position.y});
        registry.addComponent(_entity, RealEngine::Drawable{});
        registry.addComponent(
            _entity, RealEngine::Interpolation{
                         {position.x, position.y}, {position.x, position.y}, 0.f, 1.f, false});
        auto spriteSheet =
            RealEngine::AssetManager::getInstance().getSpriteSheet("spaceship_other");
        if (spriteSheet) {
            registry.addComponent(_entity, RealEngine::SpriteSheet{*spriteSheet});
        } else {
            std::cerr << "Failed to load Sprite or SpriteSheet for player" << std::endl;
        }
    }
}

Player::~Player() {}

}  // namespace rtype