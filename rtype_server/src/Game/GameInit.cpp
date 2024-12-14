/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** GameInit
*/

#include "../include/Game/GameInstance.hpp"

GameInstance::GameInstance()
    : _clock(),
      _registry(),
      _drawSystem(nullptr),
      _movementSystem(),
      _collisionSystem(),
      _aiSystem(),
      _rotationSystem(),
      _radiusSystem(),
      _healthSystem(),
      _netvarSystem(),
      _upSpaceship("../../assets/spaceship.png", sf::IntRect{0, 0, 32 * 2, 15}),
      _idleSpaceship("../../assets/spaceship.png", sf::IntRect{0, 15, 32, 15}),
      _downSpaceship("../../assets/spaceship.png", sf::IntRect{0, 15 * 2, 33 * 2, 15}),
      _eyeBomberSprite("../../assets/sprites/the_eye/bomber.png"),
      _bulletSprite("../../assets/spaceship_bullet.png") {
    _idleSpaceship.setScale(GAME_SCALE, GAME_SCALE);
    _upSpaceship.setScale(GAME_SCALE, GAME_SCALE);
    _downSpaceship.setScale(GAME_SCALE, GAME_SCALE);
    _eyeBomberSprite.setScale(GAME_SCALE, GAME_SCALE);
    _eyeBomberSprite.setOrigin(15, 10);
    _spaceshipSheet.emplace("idle", _idleSpaceship);
    _spaceshipSheet.emplace("up", _upSpaceship);
    _spaceshipSheet.emplace("down", _downSpaceship);
    init_components();
    init_systems();
    std::cout << "GameInstance created" << std::endl;
}

void GameInstance::init_components() {
    _registry.register_component<RealEngine::Position>();
    _registry.register_component<RealEngine::Velocity>();
    _registry.register_component<RealEngine::Health>();
    _registry.register_component<RealEngine::SpriteComponent>();
    _registry.register_component<RealEngine::SpriteSheet>();
    _registry.register_component<RealEngine::Drawable>();
    _registry.register_component<RealEngine::Collision>();
    _registry.register_component<RealEngine::Controllable>();
    _registry.register_component<RealEngine::Acceleration>();
    _registry.register_component<RealEngine::AI>();
    _registry.register_component<RealEngine::Rotation>();
    _registry.register_component<RealEngine::Radius>();
    _registry.register_component<RealEngine::Target>();
    _registry.register_component<RealEngine::AutoDestructible>();
    _registry.register_component<RealEngine::Damage>();
    _registry.register_component<RealEngine::Netvar>();
}

void GameInstance::init_systems() {
    // _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
    //     _movementSystem.update(registry, deltaTime);
    // });
    // _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
    //     _drawSystem.update(registry, deltaTime);
    // });
    // _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
    //     _controlSystem.update(registry, deltaTime);
    // });
    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _collisionSystem.update(registry, deltaTime);
    });
    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _aiSystem.update(registry, deltaTime);
    });
    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _rotationSystem.update(registry, deltaTime);
    });
    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _radiusSystem.update(registry);
    });
    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _destructibleSystem.update(registry, deltaTime);
    });
    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _healthSystem.update(registry, deltaTime);
    });
    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _netvarSystem.update(registry, deltaTime);
    });
}
