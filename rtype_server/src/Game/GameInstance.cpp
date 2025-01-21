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

void GameInstance::manageInGameEntities(std::vector<Map::WaveMob>       enemies_to_spawn,
                                        std::vector<RealEngine::Entity> destroyedEntities) {
    if (enemies_to_spawn.empty()) return;
    for (auto& enemy : enemies_to_spawn) {
        spawnMob(enemy.name, enemy.position, enemy.angle);
    }
    if (destroyedEntities.empty()) return;
    for (auto& entity : destroyedEntities) {
        auto* netvarContainer = _registry.getComponent<RealEngine::NetvarContainer>(entity);
        auto* position        = _registry.getComponent<RealEngine::Position>(entity);
        if (netvarContainer) {
            auto powerupDropNetvar = netvarContainer->getNetvar("powerup_drop");
            if (powerupDropNetvar && powerupDropNetvar->value.type() == typeid(float)) {
                float spawnProbability = std::any_cast<float>(powerupDropNetvar->value);
                if (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) <
                    spawnProbability / 100.0f) {
                    std::cout << "spawned powerup" << std::endl;
                    rtype::PowerUp powerup = rtype::PowerUp(_registry, {position->x, position->y},
                                                            rtype::bonusType::HEAL);
                    auto           powerupTypeNetvar = netvarContainer->getNetvar("powerup_type");
                    if (powerupTypeNetvar && powerupTypeNetvar->value.type() == typeid(int)) {
                        int powerupType = std::any_cast<int>(powerupTypeNetvar->value);
                        switch (powerupType) {
                            case 1:
                                powerup = rtype::PowerUp(_registry, {position->x, position->y},
                                                         rtype::bonusType::SHOOT);
                                break;
                            case 2:
                                powerup = rtype::PowerUp(_registry, {position->x, position->y},
                                                         rtype::bonusType::SPEED);
                                break;
                            default:
                                break;
                        }
                    }
                    addAndGetEntity(powerup.getEntity());
                }
            }
        }
    }
    _bullets.erase(std::remove_if(_bullets.begin(), _bullets.end(),
                                  [&](const auto& bullet) {
                                      return _registry.getComponent<RealEngine::Health>(bullet) ==
                                             nullptr;
                                  }),
                   _bullets.end());
}

std::vector<RealEngine::Entity> GameInstance::run(float deltaTime) {
    for (auto& mob : _enemies) {
        _movementSystem.update(_registry, mob, deltaTime);
    }

    for (auto& bullet : _bullets) {
        _movementSystem.update(_registry, bullet, deltaTime);
    }
    if (_serverVision) {
        _window->clear();
        _window->update(deltaTime);
        _drawSystem.update(_registry, deltaTime);
        _window->display();
    } else {
        _drawSystem.updateWithoutDisplay(_registry, deltaTime);
    }
    _drawSystem.updateWithoutDisplay(_registry, deltaTime);
    _aiSystem.update(_registry, deltaTime);
    _rotationSystem.update(_registry, deltaTime);
    _radiusSystem.update(_registry);
    _targetRadiusSystem.update(_registry);
    _particleSystem.update(_registry, deltaTime);
    _collisionSystem.update(_registry, deltaTime);
    _healthSystem.update(_registry, deltaTime);
    _netvarSystem.update(_registry, deltaTime);
    _destroySystem.update(_registry, deltaTime);
    auto destroyedEntities = _destroySystem.getDeadEntities();
    _netvarSystem.update(_registry, deltaTime);
    if (_gameMap) {
        _gameMap->updateLevel(deltaTime);
    }
    auto enemies_to_spawn = _gameMap->invokeLevelMobs();

    manageInGameEntities(enemies_to_spawn, destroyedEntities);

    return destroyedEntities;
};

void GameInstance::movePlayer(long int playerUuid, sf::IntRect direction, float deltaTime) {
    if (_players.empty() || _players.find(playerUuid) == _players.end()) return;

    std::shared_ptr<RealEngine::Entity> player = _players.at(playerUuid);
    auto* acceleration = _registry.getComponent<RealEngine::Acceleration>(player);
    auto* velocity     = _registry.getComponent<RealEngine::Velocity>(player);
    auto* position     = _registry.getComponent<RealEngine::Position>(player);

    if (!acceleration || !velocity || !position) return;

    if (direction.left > 0 && velocity->vx > 50) velocity->vx = 50;
    if (direction.top > 0 && velocity->vx < -50) velocity->vx = -50;
    if (direction.width > 0 && velocity->vy > 50) velocity->vy = 50;
    if (direction.height > 0 && velocity->vy < -50) velocity->vy = -50;
    if (direction.top > 0) velocity->vx += (acceleration->ax * 3 * deltaTime);
    if (direction.left > 0) velocity->vx -= (acceleration->ax * 3 * deltaTime);
    if (direction.width > 0) velocity->vy -= (acceleration->ay * 3 * deltaTime);
    if (direction.height > 0) velocity->vy += (acceleration->ay * 3 * deltaTime);
    if (direction.left == 1 && direction.top == 1) {
        velocity->vx = 0;
    }
    if (direction.width == 1 && direction.height == 1) {
        velocity->vy = 0;
    }
}

void GameInstance::startLevel() {
    if (_gameMap->getIsLevelRunning() == false) {
        _gameMap->startLevel();
    } else {
        _gameMap->stopLevel();
    }
}