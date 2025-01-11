/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** GameInstance
*/

#pragma once

#include "Engine.hpp"
#include "Game/Player/Bullet.hpp"
#include "Game/Player/Player.hpp"
#include "Mobs/Mobs.hpp"
#include "ServerMap.hpp"

class GameInstance {
   public:
    GameInstance(bool serverVision = false);
    void init_components();
    void init_systems();
    void init_mobs();
    void init_textures();
    void init_sprite_sheets();
    void init_screen_limits();

    void init_level(std::string filepath, std::string foldername);

    void handleSignal(const std::string& message);

    std::shared_ptr<RealEngine::Entity> addAndGetPlayer(sf::Vector2f position);
    std::shared_ptr<RealEngine::Entity> addAndGetEntity(sf::Vector2f position);
    std::shared_ptr<RealEngine::Entity> addAndGetBullet(sf::Vector2f position,
                                                        sf::Vector2f direction, float speed,
                                                        std::string spriteName, float damage,
                                                        int health);
    std::shared_ptr<RealEngine::Entity> addAndGetEnemy(
        std::shared_ptr<RealEngine::Entity> enemyEntity);
    void spawnMob(const std::string& mobName, const sf::Vector2f& position, float angle);

    void movePlayer(long int playerUuid, sf::IntRect direction, float deltaTime);

    void runPlayerSimulation(std::shared_ptr<RealEngine::Entity> entity, float deltaTime);
    std::vector<RealEngine::Entity> run(float deltaTime);

    RealEngine::Registry* getRegistry() { return &_registry; }
    RealEngine::Registry& getRegistryRef() { return _registry; }

    std::vector<std::shared_ptr<RealEngine::Entity>>& getSimpleMobs() { return _enemies; }
    std::shared_ptr<ServerMap>                        getMap() { return _game_map; }

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

    std::unordered_map<long int, std::shared_ptr<RealEngine::Entity>> _players;
    std::vector<std::shared_ptr<RealEngine::Entity>>                  _enemies;
    std::vector<std::shared_ptr<RealEngine::Entity>>                  _bullets;
    std::shared_ptr<ServerMap>                                        _game_map;
};