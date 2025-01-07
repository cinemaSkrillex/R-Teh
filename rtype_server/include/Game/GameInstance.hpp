/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** GameInstance
*/

#pragma once

#define WINDOWED true  // set to true to have a window for the server

#include "Engine.hpp"
#include "Game/Player/Bullet.hpp"
#include "Game/Player/Player.hpp"
#include "Mobs/Mobs.hpp"

class GameInstance {
   public:
    GameInstance();
    void init_components();
    void init_systems();
    void init_mobs();
    void init_textures();
    void init_sprites();
    void set_sprite_scales();

    void handleSignal(const std::string& message);

    std::shared_ptr<RealEngine::Entity> addAndGetPlayer(sf::Vector2f position);
    std::shared_ptr<RealEngine::Entity> addAndGetEntity(sf::Vector2f position);
    std::shared_ptr<RealEngine::Entity> addAndGetBullet(sf::Vector2f position,
                                                        sf::Vector2f direction, float speed,
                                                        std::string spriteName, float damage,
                                                        int health);
    std::shared_ptr<RealEngine::Entity> addAndGetSimpleMob(sf::Vector2f position,
                                                           sf::Vector2f direction, float speed,
                                                           float destructTimer);
    void movePlayer(long int playerUuid, sf::IntRect direction, float deltaTime);

    void runPlayerSimulation(std::shared_ptr<RealEngine::Entity> entity, float deltaTime);
    // void run(float deltaTime);
    std::vector<RealEngine::Entity> run(float deltaTime);

    RealEngine::Registry* getRegistry() { return &_registry; }
    RealEngine::Registry& getRegistryRef() { return _registry; }

    std::vector<std::shared_ptr<RealEngine::Entity>>& getSimpleMobs() { return _ennemies; }

   private:
    std::unique_ptr<RealEngine::Window> _window;
    sf::Clock                           _clock;

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
    std::vector<std::shared_ptr<RealEngine::Entity>>                  _ennemies;
    std::vector<std::shared_ptr<RealEngine::Entity>>                  _bullets;
};