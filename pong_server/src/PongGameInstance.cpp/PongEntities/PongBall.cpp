/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** PongBall.cpp
*/

#include "GameInstance/PongEntities/PongBall.hpp"

namespace pong {

static void handlePlayerCollision(RealEngine::Registry& registry, RealEngine::Entity collider,
                                  RealEngine::Entity entity) {
    auto* velocity = registry.get_component<RealEngine::Velocity>(entity);
    if (!velocity) {
        return;
    }

    velocity->vx = -velocity->vx;
}

static void handleWallCollision(RealEngine::Registry& registry, RealEngine::Entity collider,
                                RealEngine::Entity entity) {
    auto* velocity = registry.get_component<RealEngine::Velocity>(entity);
    if (!velocity) {
        return;
    }

    velocity->vy = -velocity->vy;
}

static void handleRestartCollision(RealEngine::Registry& registry, RealEngine::Entity collider,
                                   RealEngine::Entity entity) {
    auto* position        = registry.get_component<RealEngine::Position>(entity);
    auto* netvarContainer = registry.get_component<RealEngine::NetvarContainer>(entity);
    if (!position) {
        return;
    }

    if (netvarContainer) {
        if (position->x < 400) {
            netvarContainer->netvars["score_to_update"].value = 1;
        } else {
            netvarContainer->netvars["score_to_update"].value = 2;
        }
    }

    position->x = 400;
    position->y = 300;
}

static void BallCollisionHandler(RealEngine::CollisionType collisionType,
                                 RealEngine::Registry& registry, RealEngine::Entity collider,
                                 RealEngine::Entity entity) {
    switch (collisionType) {
        case RealEngine::CollisionType::PLAYER:
            handlePlayerCollision(registry, collider, entity);
            break;
        case RealEngine::CollisionType::BLOCKING:
            handleWallCollision(registry, collider, entity);
            break;
        case RealEngine::CollisionType::SOLID:
            handleRestartCollision(registry, collider, entity);
            break;
        default:
            break;
    }
}

PongBall::PongBall(RealEngine::Registry& registry, sf::Vector2f position)
    : _entity(registry.spawn_entity()) {
    registry.add_component(_entity, RealEngine::Position{position.x, position.y});
    registry.add_component(_entity, RealEngine::Velocity{0.0f, 0.0f, {300.0f, 300.0f}, 3.0f});
    registry.add_component(_entity, RealEngine::Acceleration{1000.0f, 1000.0f, 1000.0f});
    registry.add_component(_entity, RealEngine::Collision{{-1, -1, -1, -1},
                                                          "ball",
                                                          false,
                                                          RealEngine::CollisionType::OTHER,
                                                          BallCollisionHandler});
    auto sprite = RealEngine::AssetManager::getInstance().getSprite("ball");
    registry.add_component(_entity, RealEngine::SpriteComponent{*sprite});
    registry.add_component(_entity, RealEngine::Drawable{});
    registry.add_component(_entity, RealEngine::Score{1});
    registry.add_component(
        _entity, RealEngine::Interpolation{
                     {position.x, position.y}, {position.x, position.y}, 0.f, 1.f, false});
    registry.add_component(
        _entity, RealEngine::NetvarContainer{{
                     {"sprite_name", {"string", "sprite_name", std::string("ball"), nullptr}},
                     {"score_to_update", {"int", "score", 0, nullptr}},
                 }});
}

PongBall::~PongBall() {}

}  // namespace pong