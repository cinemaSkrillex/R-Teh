#pragma once

#include "../../../game_engine/include/Engine.hpp"

class GameInstance {
   public:
    GameInstance();
    void init_components();
    void init_systems();
    void run();
    void handleSignal(const std::string& message);
    void addPlayer(int playerPort, sf::Vector2f cameraPosition);
    void movePlayer(int playerPort, sf::Vector2f direction);

   private:
    float                       _deltaTime;
    sf::Clock                   _clock;
    RealEngine::Registry        _registry;
    RealEngine::MovementSystem  _movementSystem;
    RealEngine::CollisionSystem _collisionSystem;
    RealEngine::AISystem        _aiSystem;
    RealEngine::RotationSystem  _rotationSystem;
    RealEngine::RadiusSystem    _radiusSystem;

    std::unordered_map<int, RealEngine::Entity> _players;
    std::vector<RealEngine::Entity>             _groundBlocksEntities;
};