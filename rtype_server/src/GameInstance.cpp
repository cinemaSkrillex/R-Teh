#include "../include/Game/GameInstance.hpp"

GameInstance::GameInstance():
      _clock(),
      _registry(),
      _movementSystem(),
      _collisionSystem(),
      _aiSystem(),
      _rotationSystem(),
      _radiusSystem() {
    init_components();
    init_systems();
}

void GameInstance::init_components() {
    _registry.register_component<RealEngine::Position>();
    _registry.register_component<RealEngine::Velocity>();
    _registry.register_component<RealEngine::Health>();
    _registry.register_component<RealEngine::Collision>();
    _registry.register_component<RealEngine::Acceleration>();
    _registry.register_component<RealEngine::AI>();
    _registry.register_component<RealEngine::Rotation>();
    _registry.register_component<RealEngine::Radius>();
    _registry.register_component<RealEngine::Target>();
}

void GameInstance::init_systems() {
    _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
        _movementSystem.update(registry, deltaTime);
    });
    // _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
    //     _collisionSystem.update(registry, deltaTime);
    // });
    // _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
    //     _aiSystem.update(registry, deltaTime);
    // });
    // _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
    //     _rotationSystem.update(registry, deltaTime);
    // });
    // _registry.add_system<>([this](RealEngine::Registry& registry, float deltaTime) {
    //     _radiusSystem.update(registry);
    // });
}

void GameInstance::run(float deltaTime) { 
        // _registry.update(deltaTime);
        _movementSystem.update(_registry, deltaTime);
    };

RealEngine::Entity* GameInstance::addPlayer(long int playerUuid, sf::Vector2f position) {
    RealEngine::Entity player = _registry.spawn_entity();
    _registry.add_component(player, RealEngine::Position{position.x, position.y});
    _registry.add_component(player, RealEngine::Velocity{0.0f, 0.0f, {300.0f, 300.0f}, 3.f});
    _registry.add_component(player, RealEngine::Acceleration{1000.0f, 1000.0f, 1000.0f});
    _players.emplace(playerUuid, player);
    return &_players.at(playerUuid);
}

void GameInstance::movePlayer(long int playerUuid, sf::Vector2f direction, float deltaTime) {
    if (_players.find(playerUuid) == _players.end()) return;

    RealEngine::Entity player       = _players.at(playerUuid);
    auto*              acceleration = _registry.get_component<RealEngine::Acceleration>(player);
    auto*              velocity     = _registry.get_component<RealEngine::Velocity>(player);
    auto*              position     = _registry.get_component<RealEngine::Position>(player);

    if (direction.x < 0 && velocity->vx > 50) velocity->vx = 50;
    if (direction.x > 0 && velocity->vx < -50) velocity->vx = -50;
    if (direction.y < 0 && velocity->vy > 50) velocity->vy = 50;
    if (direction.y > 0 && velocity->vy < -50) velocity->vy = -50;
    velocity->vx += (acceleration->ax * 3 * deltaTime) * direction.x;
    velocity->vy += (acceleration->ay * 3 * deltaTime) * direction.y;
}

void GameInstance::handleSignal(const std::string& message) {
    // Do something
}