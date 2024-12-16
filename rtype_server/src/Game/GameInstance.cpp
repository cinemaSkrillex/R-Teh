/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** GameInstance
*/

#include "../include/Game/GameInstance.hpp"

void GameInstance::runPlayerSimulation(std::shared_ptr<RealEngine::Entity> entity,
                                       float                               deltaTime) {
    _movementSystem.update(_registry, entity, deltaTime);
};

void GameInstance::run(float deltaTime) {
    // _registry.update(deltaTime);
    _drawSystem.updateWithoutDisplay(_registry, deltaTime);
    _aiSystem.update(_registry, deltaTime);
    _rotationSystem.update(_registry, deltaTime);
    // _radiusSystem.update(_registry, deltaTime);
    _destructibleSystem.update(_registry, deltaTime);
    _collisionSystem.update(_registry, deltaTime);
    _healthSystem.update(_registry, deltaTime);
    _netvarSystem.update(_registry, deltaTime);
    // for (auto& mob : _simpleMobs) {
    //     if (_registry.get_component<RealEngine::Health>(mob) == nullptr) {
    //         _simpleMobs.erase(std::remove(_simpleMobs.begin(), _simpleMobs.end(), mob),
    //                           _simpleMobs.end());
    //         continue;
    //     }
    //     _movementSystem.update(_registry, mob, deltaTime);
    // }
    _simpleMobs.erase(std::remove_if(_simpleMobs.begin(), _simpleMobs.end(),
                                     [&](const auto& mob) {
                                         return _registry.get_component<RealEngine::Health>(mob) ==
                                                nullptr;
                                     }),
                      _simpleMobs.end());

    _bullets.erase(std::remove_if(_bullets.begin(), _bullets.end(),
                                  [&](const auto& bullet) {
                                      return _registry.get_component<RealEngine::Health>(bullet) ==
                                             nullptr;
                                  }),
                   _bullets.end());

    // Then update remaining mobs
    for (auto& mob : _simpleMobs) {
        _movementSystem.update(_registry, mob, deltaTime);
    }

    // Then update remaining bullets
    for (auto& bullet : _bullets) {
        _movementSystem.update(_registry, bullet, deltaTime);
    }
    // for (auto& bullet : _bullets) {
    //     if (_registry.get_component<RealEngine::Health>(bullet) == nullptr) {
    //         _bullets.erase(std::remove(_bullets.begin(), _bullets.end(), bullet),
    //                        _bullets.end());
    //         continue;
    //     }
    //     _movementSystem.update(_registry, bullet, deltaTime);
    // }
};

std::shared_ptr<RealEngine::Entity> GameInstance::addAndGetPlayer(long int     playerUuid,
                                                                  sf::Vector2f position) {
    rtype::Player player(_registry, position, _spaceshipSheet);
    std::cout << position.x << " " << position.y << std::endl;
    auto playerEntity = player.getEntity();
    _players.emplace(playerUuid, playerEntity);
    // RealEngine::Entity player = _registry.spawn_entity();
    // _registry.add_component(player, RealEngine::Position{position.x, position.y});
    // _registry.add_component(player, RealEngine::Velocity{0.0f, 0.0f, {300.0f, 300.0f}, 3.f});
    // _registry.add_component(player, RealEngine::Acceleration{1000.0f, 1000.0f, 1000.0f});
    // _players.emplace(playerUuid, player);
    return _players.at(playerUuid);
}

std::shared_ptr<RealEngine::Entity> GameInstance::addAndGetEntity(long int     uuid,
                                                                  sf::Vector2f position) {
    // auto eyeBomber = rtype::EyeBomber(_registry, position, _eyeBomberSprite);
    // auto entity    = eyeBomber.getEntity();
    // _ennemies.emplace(uuid, entity);
    // return entity;
    auto entity = _registry.spawn_entity();
    _simpleMobs.push_back(entity);
    return entity;
}

std::shared_ptr<RealEngine::Entity> GameInstance::addAndGetBullet(sf::Vector2f position,
                                                                  sf::Vector2f direction,
                                                                  float        speed) {
    std::shared_ptr<rtype::Bullet> bullet =
        std::make_shared<rtype::Bullet>(_registry, position, direction, speed, _bulletSprite);
    _bullets.push_back(bullet->getEntity());
    return bullet->getEntity();
}

std::shared_ptr<RealEngine::Entity> GameInstance::addAndGetSimpleMob(sf::Vector2f position,
                                                                     sf::Vector2f direction,
                                                                     float        speed) {
    std::shared_ptr<rtype::SimpleMob> mob =
        std::make_shared<rtype::SimpleMob>(_registry, position, direction, speed, _simpleMobSprite);
    _simpleMobs.push_back(mob->getEntity());
    return _simpleMobs.back();
}

void GameInstance::movePlayer(long int playerUuid, sf::Vector2f direction, float deltaTime) {
    if (_players.find(playerUuid) == _players.end()) return;

    std::shared_ptr<RealEngine::Entity> player = _players.at(playerUuid);
    auto* acceleration = _registry.get_component<RealEngine::Acceleration>(player);
    auto* velocity     = _registry.get_component<RealEngine::Velocity>(player);
    auto* position     = _registry.get_component<RealEngine::Position>(player);

    if (direction.x < 0 && velocity->vx > 50) velocity->vx = 50;
    if (direction.x > 0 && velocity->vx < -50) velocity->vx = -50;
    if (direction.y < 0 && velocity->vy > 50) velocity->vy = 50;
    if (direction.y > 0 && velocity->vy < -50) velocity->vy = -50;

    velocity->vx += (acceleration->ax * 3 * deltaTime) * direction.x;
    velocity->vy += (acceleration->ay * 3 * deltaTime) * direction.y;
}

void GameInstance::handleSignal(const std::string& message) {
    // Do somethin
}