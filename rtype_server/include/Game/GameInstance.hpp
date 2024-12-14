/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** GameInstance
*/

#pragma once

#include "Engine.hpp"
#include "EyeBomber.hpp"
#include "PlayerEntity.hpp"

class GameInstance {
   public:
    GameInstance();
    void init_components();
    void init_systems();

    void handleSignal(const std::string& message);

    std::shared_ptr<RealEngine::Entity> addAndGetPlayer(long int playerUuid, sf::Vector2f position);
    std::shared_ptr<RealEngine::Entity> addAndGetEntity(sf::Vector2f position);
    void movePlayer(long int playerUuid, sf::Vector2f direction, float deltaTime);

    void runPlayerSimulation(std::shared_ptr<RealEngine::Entity> entity, float deltaTime);
    void run(float deltaTime);

    RealEngine::Registry* getRegistry() { return &_registry; }
    RealEngine::Registry& getRegistryRef() { return _registry; }

   private:
    sf::Clock                      _clock;
    RealEngine::Registry           _registry;
    RealEngine::DrawSystem         _drawSystem;
    RealEngine::MovementSystem     _movementSystem;
    RealEngine::CollisionSystem    _collisionSystem;
    RealEngine::AISystem           _aiSystem;
    RealEngine::RotationSystem     _rotationSystem;
    RealEngine::RadiusSystem       _radiusSystem;
    RealEngine::DestructibleSystem _destructibleSystem;
    RealEngine::HealthSystem       _healthSystem;
    RealEngine::NetvarSystem       _netvarSystem;

    std::unordered_map<long int, std::shared_ptr<RealEngine::Entity>> _players;
    std::vector<std::unique_ptr<rtype::EyeBomber>>                    _ennemies;
    RealEngine::Sprite                                                _eyeBomberSprite;
    RealEngine::Sprite                                                _upSpaceship;
    RealEngine::Sprite                                                _idleSpaceship;
    RealEngine::Sprite                                                _downSpaceship;
    std::unordered_map<std::string, RealEngine::Sprite>               _spaceshipSheet;
    std::vector<std::shared_ptr<RealEngine::Entity>>                  _groundBlocksEntities;
};