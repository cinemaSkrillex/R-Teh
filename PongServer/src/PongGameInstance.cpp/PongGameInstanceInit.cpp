/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** PongGameInstanceInit.cpp
*/

#include "GameInstance/PongGameInstance.hpp"

void PongGameInstance::initComponents() {
    _registry.registerComponent<RealEngine::Position>();
    _registry.registerComponent<RealEngine::Velocity>();
    _registry.registerComponent<RealEngine::SpriteComponent>();
    _registry.registerComponent<RealEngine::SpriteSheet>();
    _registry.registerComponent<RealEngine::Drawable>();
    _registry.registerComponent<RealEngine::Collision>();
    _registry.registerComponent<RealEngine::Controllable>();
    _registry.registerComponent<RealEngine::Acceleration>();
    _registry.registerComponent<RealEngine::Netvar>();
    _registry.registerComponent<RealEngine::NetvarContainer>();
    _registry.registerComponent<RealEngine::Particle>();
    _registry.registerComponent<RealEngine::ParticleEmitter>();
    _registry.registerComponent<RealEngine::Score>();
    _registry.registerComponent<RealEngine::Interpolation>();
}

void PongGameInstance::initSystems() {
    _registry.addSystem<>([this](RealEngine::Registry &registry, float deltaTime) {
        _netvarSystem.update(registry, deltaTime);
    });

    _registry.addSystem<>([this](RealEngine::Registry &registry, float deltaTime) {
        _collisionSystem.update(registry, deltaTime);
    });

    _registry.addSystem<>([this](RealEngine::Registry &registry, float deltaTime) {
        _movementSystem.update(registry, deltaTime);
    });

    _registry.addSystem<>([this](RealEngine::Registry &registry, float deltaTime) {
        _drawSystem.update(registry, deltaTime);
    });

    _registry.addSystem<>([this](RealEngine::Registry &registry, float deltaTime) {
        _drawSystem.update(registry, deltaTime);
    });
}

void PongGameInstance::initTextures() {
    std::string path = "../../assets/sprites/pong/";

    auto &AssetManagerInstance = RealEngine::AssetManager::getInstance();

    AssetManagerInstance.loadSpriteTextureAndScale("background", path + "background.png",
                                                   {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("ball", path + "ball.png",
                                                   {GAME_SCALE, GAME_SCALE});
    AssetManagerInstance.loadSpriteTextureAndScale("player_bar", path + "bar_idle.png",
                                                   {GAME_SCALE, GAME_SCALE});
}

void PongGameInstance::initSpriteSheets() {
    // no sprite sheets for now
}

void PongGameInstance::initScreenLimits() {
    std::shared_ptr<RealEngine::Entity> topWall = _registry.spawnEntity();
    _registry.addComponent(
        topWall,
        RealEngine::Collision{
            {0, -20, VIEW_WIDTH, 20}, "limits", false, RealEngine::CollisionType::BLOCKING});

    std::shared_ptr<RealEngine::Entity> bottomWall = _registry.spawnEntity();
    _registry.addComponent(bottomWall, RealEngine::Collision{{0, VIEW_HEIGHT, VIEW_WIDTH, 20},
                                                             "limits",
                                                             false,
                                                             RealEngine::CollisionType::BLOCKING});

    std::shared_ptr<RealEngine::Entity> leftWall = _registry.spawnEntity();
    _registry.addComponent(
        leftWall,
        RealEngine::Collision{
            {-20, 0, 20, VIEW_HEIGHT}, "limits", false, RealEngine::CollisionType::SOLID});

    std::shared_ptr<RealEngine::Entity> rightWall = _registry.spawnEntity();
    _registry.addComponent(
        rightWall,
        RealEngine::Collision{
            {VIEW_WIDTH, 0, 20, VIEW_HEIGHT}, "limits", false, RealEngine::CollisionType::SOLID});

    std::shared_ptr<RealEngine::Entity> entitiesRect = _registry.spawnEntity();
    _registry.addComponent(entitiesRect,
                           RealEngine::Collision{{-100, -50, VIEW_WIDTH + 200, VIEW_HEIGHT + 100},
                                                 "limits",
                                                 false,
                                                 RealEngine::CollisionType::SCREEN});
}