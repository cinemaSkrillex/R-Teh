/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** GameInstance
*/

#pragma once

#include "Engine.hpp"
#include "Game/Particles.hpp"
#include "Game/Player/Bullet.hpp"
#include "Game/Player/Player.hpp"
#include "Game/PowerUp.hpp"
#include "Mobs/Mobs.hpp"
#include "ServerMap.hpp"

class GameInstance {
   public:
    GameInstance(bool serverVision = false);
    void initComponents();
    void initSystems();
    void initTextures();
    void initSpriteSheets();
    void initScreenLimits();
    void startLevel();

    void initLevel(std::string filepath, std::string foldername);

    std::shared_ptr<RealEngine::Entity> addAndGetPlayer(sf::Vector2f position);
    std::shared_ptr<RealEngine::Entity> addAndGetEntity(std::shared_ptr<RealEngine::Entity> entity);
    std::shared_ptr<RealEngine::Entity> addAndGetBullet(sf::Vector2f position, float speed,
                                                        std::string spriteName, float damage,
                                                        int health, size_t playerID);

    void movePlayer(long int playerUuid, sf::IntRect direction, float deltaTime);

    void runPlayerSimulation(std::shared_ptr<RealEngine::Entity> entity, float deltaTime);
    std::vector<RealEngine::Entity> run(float deltaTime);
    void                            manageInGameEntities(std::vector<Map::WaveMob>       enemies_to_spawn,
                                                         std::vector<RealEngine::Entity> destroyedEntities);
    void spawnMob(std::string mobName, const sf::Vector2f position, float angle);

    RealEngine::Registry* getRegistry() { return &_registry; }
    RealEngine::Registry& getRegistryRef() { return _registry; }

    std::vector<std::shared_ptr<RealEngine::Entity>>& getSimpleMobs() { return _enemies; }
    std::shared_ptr<ServerMap>                        getMap() {
        if (!_gameMap) {
            std::cerr << "getMap error: _gameMap is null" << std::endl;
        }
        return _gameMap;
    }

   private:
    bool                                _serverVision;
    std::unique_ptr<RealEngine::Window> _window;
    sf::Clock                           _clock;

    RealEngine::Registry           _registry;
    RealEngine::DrawSystem         _drawSystem;
    RealEngine::MovementSystem     _movementSystem;
    RealEngine::CollisionSystem    _collisionSystem;
    RealEngine::AISystem           _aiSystem;
    RealEngine::RotationSystem     _rotationSystem;
    RealEngine::RadiusSystem       _radiusSystem;
    RealEngine::DestroySystem      _destroySystem;
    RealEngine::HealthSystem       _healthSystem;
    RealEngine::NetvarSystem       _netvarSystem;
    RealEngine::TargetRadiusSystem _targetRadiusSystem;
    RealEngine::ParticleSystem     _particleSystem;

    std::unordered_map<long int, std::shared_ptr<RealEngine::Entity>> _players;
    std::vector<std::shared_ptr<RealEngine::Entity>>                  _enemies;
    std::vector<std::shared_ptr<RealEngine::Entity>>                  _bullets;
    std::shared_ptr<ServerMap>                                        _gameMap;
};