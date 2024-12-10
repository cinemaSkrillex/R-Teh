#pragma once

#include "../../../game_engine/include/Engine.hpp"

class GameInstance {
   public:
    GameInstance();
    void                  init_components();
    void                  init_systems();
    void                  run(float deltaTime);
    void                  handleSignal(const std::string& message);
    RealEngine::Entity*   addPlayer(long int playerUuid, sf::Vector2f position);
    void                  movePlayer(long int playerUuid, sf::Vector2f direction, float deltaTime);
    RealEngine::Registry* getRegistry() { return &_registry; }

   private:
    sf::Clock                   _clock;
    RealEngine::Registry        _registry;
    RealEngine::MovementSystem  _movementSystem;
    RealEngine::CollisionSystem _collisionSystem;
    RealEngine::AISystem        _aiSystem;
    RealEngine::RotationSystem  _rotationSystem;
    RealEngine::RadiusSystem    _radiusSystem;

    std::unordered_map<long int, RealEngine::Entity> _players;
    std::vector<RealEngine::Entity>             _groundBlocksEntities;
};