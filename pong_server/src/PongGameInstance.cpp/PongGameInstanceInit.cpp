/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** PongGameInstanceInit.cpp
*/

#include "GameInstance/PongGameInstance.hpp"

void PongGameInstance::init_components() {
    _registry.register_component<RealEngine::Position>();
    _registry.register_component<RealEngine::Velocity>();
    _registry.register_component<RealEngine::SpriteComponent>();
    _registry.register_component<RealEngine::SpriteSheet>();
    _registry.register_component<RealEngine::Drawable>();
    _registry.register_component<RealEngine::Collision>();
    _registry.register_component<RealEngine::Controllable>();
    _registry.register_component<RealEngine::Acceleration>();
    _registry.register_component<RealEngine::Netvar>();
    _registry.register_component<RealEngine::NetvarContainer>();
    _registry.register_component<RealEngine::Particle>();
    _registry.register_component<RealEngine::ParticleEmitter>();
    _registry.register_component<RealEngine::Score>();
    _registry.register_component<RealEngine::Interpolation>();
}

void PongGameInstance::init_systems() {
    _registry.add_system<>([this](RealEngine::Registry &registry, float deltaTime) {
        _netvarSystem.update(registry, deltaTime);
    });

    _registry.add_system<>([this](RealEngine::Registry &registry, float deltaTime) {
        _collisionSystem.update(registry, deltaTime);
    });

    _registry.add_system<>([this](RealEngine::Registry &registry, float deltaTime) {
        _movementSystem.update(registry, deltaTime);
    });

    _registry.add_system<>([this](RealEngine::Registry &registry, float deltaTime) {
        _drawSystem.update(registry, deltaTime);
    });

    _registry.add_system<>([this](RealEngine::Registry &registry, float deltaTime) {
        _drawSystem.update(registry, deltaTime);
    });
}

void PongGameInstance::init_textures() {
    std::string path = "../../assets/sprites/pong/";

    auto &AssetManagerInstance = RealEngine::AssetManager::getInstance();

    AssetManagerInstance.loadSpriteTextureAndScale("background", path + "background.png",
                                                   {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("ball", path + "ball.png",
                                                   {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("player_bar", path + "bar_idle.png",
                                                   {GAME_SCALE, GAME_SCALE});
}

void PongGameInstance::init_sprite_sheets() {
    // no sprite sheets for now
}

void PongGameInstance::init_screen_limits() {
    std::shared_ptr<RealEngine::Entity> topWall = _registry.spawn_entity();
    _registry.add_component(
        topWall,
        RealEngine::Collision{
            {0, -20, VIEW_WIDTH, 20}, "limits", false, RealEngine::CollisionType::BLOCKING});

    std::shared_ptr<RealEngine::Entity> bottomWall = _registry.spawn_entity();
    _registry.add_component(bottomWall, RealEngine::Collision{{0, VIEW_HEIGHT, VIEW_WIDTH, 20},
                                                              "limits",
                                                              false,
                                                              RealEngine::CollisionType::BLOCKING});

    std::shared_ptr<RealEngine::Entity> leftWall = _registry.spawn_entity();
    _registry.add_component(
        leftWall,
        RealEngine::Collision{
            {-20, 0, 20, VIEW_HEIGHT}, "limits", false, RealEngine::CollisionType::SOLID});

    std::shared_ptr<RealEngine::Entity> rightWall = _registry.spawn_entity();
    _registry.add_component(rightWall, RealEngine::Collision{{VIEW_WIDTH, 0, 20, VIEW_HEIGHT},
                                                             "limits",
                                                             false,
                                                             RealEngine::CollisionType::SOLID});

    std::shared_ptr<RealEngine::Entity> entitiesRect = _registry.spawn_entity();
    _registry.add_component(entitiesRect,
                            RealEngine::Collision{{-100, -50, VIEW_WIDTH + 200, VIEW_HEIGHT + 100},
                                                  "limits",
                                                  false,
                                                  RealEngine::CollisionType::SCREEN});
}