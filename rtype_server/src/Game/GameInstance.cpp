/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** GameInstance
*/

#include "../include/Game/GameInstance.hpp"

void GameInstance::run(RealEngine::Entity& entity, float deltaTime) {
    // _registry.update(deltaTime);
    // _movementSystem.update(_registry, entity, deltaTime);
    _registry.run_systems(deltaTime);
};

RealEngine::Entity* GameInstance::addAndGetPlayer(long int playerUuid, sf::Vector2f position) {
    rtype::Player player(_registry, position, _spaceshipSheet);
    _players.emplace(playerUuid, player.getEntity());
    // RealEngine::Entity player = _registry.spawn_entity();
    // _registry.add_component(player, RealEngine::Position{position.x, position.y});
    // _registry.add_component(player, RealEngine::Velocity{0.0f, 0.0f, {300.0f, 300.0f}, 3.f});
    // _registry.add_component(player, RealEngine::Acceleration{1000.0f, 1000.0f, 1000.0f});
    // _players.emplace(playerUuid, player);
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