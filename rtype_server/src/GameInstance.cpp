#include "../include/Game/GameInstance.hpp"

GameInstance::GameInstance()
    : _deltaTime(0.f),
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
}

void GameInstance::run() {
    while (true) {
        _deltaTime                            = _clock.restart().asSeconds();
        const std::string serverEventsMessage = "Event:New_client Uuid:123 Position:(20,60)";
        handleSignal(serverEventsMessage);
        _registry.run_systems(_deltaTime);
    }
}

RealEngine::Entity* GameInstance::addPlayer(long int playerUuid, sf::Vector2f position) {
    RealEngine::Entity player = _registry.spawn_entity();
    _registry.add_component(player, RealEngine::Position{position.x, position.y});
    _registry.add_component(player, RealEngine::Velocity{0.0f, 0.0f, {1000.0f, 1000.0f}, 0.0f});
    _players.emplace(playerUuid, player);
    return &_players.at(playerUuid);
}

void GameInstance::movePlayer(int playerPort, sf::Vector2f direction) {
    if (_players.find(playerPort) == _players.end()) return;

    RealEngine::Entity player       = _players.at(playerPort);
    auto*              acceleration = _registry.get_component<RealEngine::Acceleration>(player);
    auto*              velocity     = _registry.get_component<RealEngine::Velocity>(player);

    if (direction.x < 0 && velocity->vx > 50) velocity->vx = 50;
    if (direction.x > 0 && velocity->vx < -50) velocity->vx = -50;
    if (direction.y < 0 && velocity->vy > 50) velocity->vy = 50;
    if (direction.y > 0 && velocity->vy < -50) velocity->vy = -50;
    velocity->vx += (acceleration->ax * 3 * _deltaTime) * direction.x;
    velocity->vy += (acceleration->ay * 3 * _deltaTime) * direction.y;
}

void GameInstance::handleSignal(const std::string& message) {
    // Do something
}