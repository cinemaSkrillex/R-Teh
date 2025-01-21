/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** PongPlayer.cpp
*/

#include "GameInstance/PongEntities/PongPlayer.hpp"

namespace pong {

static void handleBallCollision(RealEngine::Registry& registry, RealEngine::Entity collider,
                                RealEngine::Entity entity) {
    // nothing but could play song or create particules
}

void handleWallStop(RealEngine::Registry& registry, RealEngine::Entity collider,
                    RealEngine::Entity entity) {
    auto* playerCollision   = registry.getComponent<RealEngine::Collision>(entity);
    auto* colliderCollision = registry.getComponent<RealEngine::Collision>(collider);
    auto* entityPosition    = registry.getComponent<RealEngine::Position>(entity);

    if (!playerCollision || !colliderCollision || !entityPosition) {
        return;
    }

    sf::FloatRect entityBounds   = playerCollision->bounds;
    sf::FloatRect colliderBounds = colliderCollision->bounds;

    float overlapLeft   = (entityBounds.left + entityBounds.width) - colliderBounds.left;
    float overlapRight  = (colliderBounds.left + colliderBounds.width) - entityBounds.left;
    float overlapTop    = (entityBounds.top + entityBounds.height) - colliderBounds.top;
    float overlapBottom = (colliderBounds.top + colliderBounds.height) - entityBounds.top;

    float correctionX = (overlapLeft < overlapRight) ? -overlapLeft : overlapRight;
    float correctionY = (overlapTop < overlapBottom) ? -overlapTop : overlapBottom;

    if (std::abs(correctionX) < std::abs(correctionY)) {
        entityPosition->x += correctionX;
    } else {
        entityPosition->y += correctionY;
    }
}

static void playerCollisionHandler(RealEngine::CollisionType collisionType,
                                   RealEngine::Registry& registry, RealEngine::Entity collider,
                                   RealEngine::Entity entity) {
    switch (collisionType) {
        case RealEngine::CollisionType::OTHER:
            handleBallCollision(registry, collider, entity);
            break;
        case RealEngine::CollisionType::BLOCKING:
            handleWallStop(registry, collider, entity);
            break;
        case RealEngine::CollisionType::SOLID:
            handleWallStop(registry, collider, entity);
            break;
        default:
            break;
    }
}

PongPlayer::PongPlayer(RealEngine::Registry& registry, sf::Vector2f position, size_t id)
    : _entity(registry.spawnEntity()) {
    registry.addComponent(_entity, RealEngine::Position{position.x, position.y});
    registry.addComponent(_entity, RealEngine::Velocity{0.0f, 0.0f, {300.0f, 300.0f}, 3.0f});
    registry.addComponent(_entity, RealEngine::Acceleration{1000.0f, 1000.0f, 1000.0f});
    registry.addComponent(_entity, RealEngine::Controllable{});
    registry.addComponent(_entity, RealEngine::Collision{{-1, -1, -1, -1},
                                                         "player_bar",
                                                         false,
                                                         RealEngine::CollisionType::PLAYER,
                                                         playerCollisionHandler});
    auto sprite = RealEngine::AssetManager::getInstance().getSprite("player_bar");
    registry.addComponent(_entity, RealEngine::SpriteComponent{*sprite});
    registry.addComponent(_entity, RealEngine::Drawable{});
    registry.addComponent(_entity, RealEngine::Score{0});
    registry.addComponent(_entity,
                          RealEngine::Interpolation{
                              {position.x, position.y}, {position.x, position.y}, 0.f, 1.f, false});
    registry.addComponent(
        _entity, RealEngine::NetvarContainer{{
                     {"player_index", {"int", "player_index", static_cast<int>(id), nullptr}},
                     {"new_entity", {"bool", "new_entity", true, nullptr}},
                 }});
}

PongPlayer::~PongPlayer() {}

}  // namespace pong