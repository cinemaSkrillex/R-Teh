/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** GameInstance
*/

#include "../../include/Game/GameInstance.hpp"

void GameInstance::runPlayerSimulation(std::shared_ptr<RealEngine::Entity> entity,
                                       float                               deltaTime) {
    _movementSystem.update(_registry, entity, deltaTime);
};

std::vector<RealEngine::Entity> GameInstance::run(float deltaTime) {
    // _registry.update(deltaTime);
    // Then update remaining mobs
    if (_serverVision) {
        _window->clear();
        _window->update();
        _drawSystem.update(_registry, deltaTime);
        _window->display();
    } else {
        _drawSystem.updateWithoutDisplay(_registry, deltaTime);
    }
    _drawSystem.updateWithoutDisplay(_registry, deltaTime);
    _aiSystem.update(_registry, deltaTime);
    _rotationSystem.update(_registry, deltaTime);
    // _radiusSystem.update(_registry, deltaTime);
    _targetRadiusSystem.update(_registry);
    _collisionSystem.update(_registry, deltaTime);
    _healthSystem.update(_registry, deltaTime);
    _netvarSystem.update(_registry, deltaTime);
    auto destroyedEntities = _destroySystem.update(_registry, deltaTime);

    _netvarSystem.update(_registry, deltaTime);
    _game_map->updateLevel(deltaTime);
    _enemies.erase(
        std::remove_if(
            _enemies.begin(), _enemies.end(),
            [&](const auto& mob) {
                std::vector<RealEngine::Netvar*> netvars =
                    _registry.get_components<RealEngine::Netvar>(mob);
                for (auto& netvar : netvars) {
                    if (netvar->name != "dropChance") continue;
                    netvar->value = std::any_cast<float>(netvar->value);
                    if (std::any_cast<float>(netvar->value) < 0) {
                        auto powerup  = _registry.spawn_entity();
                        auto position = _registry.get_component<RealEngine::Position>(mob);
                        _registry.add_component(powerup,
                                                RealEngine::Position{position->x, position->y});
                        _registry.add_component(
                            powerup, RealEngine::Collision{{0.f, 0.f, 12.f, 12.f},
                                                           "powerup_shoot",
                                                           false,
                                                           RealEngine::CollisionType::PICKABLE,
                                                           nullptr});
                    }
                }
                return _registry.get_component<RealEngine::Health>(mob) == nullptr;
            }),
        _enemies.end());

    _bullets.erase(std::remove_if(_bullets.begin(), _bullets.end(),
                                  [&](const auto& bullet) {
                                      return _registry.get_component<RealEngine::Health>(bullet) ==
                                             nullptr;
                                  }),
                   _bullets.end());

    for (auto& mob : _enemies) {
        _movementSystem.update(_registry, mob, deltaTime);
    }

    // Then update remaining bullets
    for (auto& bullet : _bullets) {
        _movementSystem.update(_registry, bullet, deltaTime);
    }

    return destroyedEntities;
};

void GameInstance::movePlayer(long int playerUuid, sf::IntRect direction, float deltaTime) {
    if (_players.find(playerUuid) == _players.end()) return;

    std::shared_ptr<RealEngine::Entity> player = _players.at(playerUuid);
    auto* acceleration = _registry.get_component<RealEngine::Acceleration>(player);
    auto* velocity     = _registry.get_component<RealEngine::Velocity>(player);
    auto* position     = _registry.get_component<RealEngine::Position>(player);

    if (!acceleration || !velocity || !position) return;

    if (direction.left > 0 && velocity->vx > 50) velocity->vx = 50;
    if (direction.top > 0 && velocity->vx < -50) velocity->vx = -50;
    if (direction.width > 0 && velocity->vy > 50) velocity->vy = 50;
    if (direction.height > 0 && velocity->vy < -50) velocity->vy = -50;
    if (direction.top > 0) velocity->vx += (acceleration->ax * 3 * deltaTime);
    if (direction.left > 0) velocity->vx -= (acceleration->ax * 3 * deltaTime);
    if (direction.width > 0) velocity->vy -= (acceleration->ay * 3 * deltaTime);
    if (direction.height > 0) velocity->vy += (acceleration->ay * 3 * deltaTime);
}

void GameInstance::handleSignal(const std::string& message) {
    // Do somethin
}