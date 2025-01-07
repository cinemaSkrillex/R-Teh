/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** Game
*/

#ifndef GAME_HPP_
#define GAME_HPP_

#include <SFML/Graphics.hpp>
#include <chrono>
#include <regex>

#include "Client/UDPClient.hpp"
#include "Controls.hpp"
#include "Engine.hpp"
#include "Game/Background.hpp"
#include "Game/Mobs/Mobs.hpp"
#include "Game/Player/Bullet.hpp"
#include "Game/Player/Player.hpp"
#include "Log.hpp"
#include "Macros.hpp"
#include "PlayerUtils.hpp"
#include "shared/RtypeServerProtocol.hpp"

namespace rtype {
class Game {
   public:
    Game(std::shared_ptr<UDPClient> clientUDP, unsigned short client_port);
    ~Game();
    void                                run();
    void                                setDeltaTime(float deltaTime) { _deltaTime = deltaTime; }
    RealEngine::Entity                  createEntity();
    void                                handleSignal(std::array<char, 800> signal);
    std::shared_ptr<RealEngine::Entity> add_player(long int player_uuid, sf::Vector2f position);
    std::shared_ptr<RealEngine::Entity> add_mob(long int enemy_uuid, sf::Vector2f position);
    sf::IntRect                         getPlayerNormalizedDirection();
    // void add_component_to_entity(RealEngine::Entity entity, RealEngine::Position position);

    std::shared_ptr<UDPClient> _clientUDP;

   private:
    // initialization functions (src/Game/GameInit.cpp)
    void init_all_game();
    void init_registry();
    void init_controls();
    void init_systems();
    void init_textures();
    void init_sprites();

    void register_components();
    void bind_keys();
    void set_action_handlers();
    void add_systems();
    void set_sprite_scales();
    void set_sprite_opacity();

    // GameSignals
    void handleNewClient(RTypeProtocol::PlayerMoveMessage parsedPacket);
    void handleSynchronize(RTypeProtocol::SynchronizeMessage parsedPacket);
    void handlePlayerMove(RTypeProtocol::PlayerMoveMessage parsedPacket);
    void handleNewEntity(RTypeProtocol::NewEntityMessage parsedPacket);
    void handleDestroyEntity(RTypeProtocol::DestroyEntityMessage parsedPacket);

    float              _deltaTime = 0.f;
    RealEngine::Window _window;
    RealEngine::View   _view;
    sf::Clock          _clock;

    RealEngine::Registry              _registry;
    RealEngine::LagCompensationSystem _lagCompensationSystem;
    RealEngine::DrawSystem            _drawSystem;
    RealEngine::ControlSystem         _controlSystem;
    RealEngine::MovementSystem        _movementSystem;
    RealEngine::CollisionSystem       _collisionSystem;
    RealEngine::AISystem              _aiSystem;
    RealEngine::RotationSystem        _rotationSystem;
    RealEngine::RadiusSystem          _radiusSystem;
    RealEngine::DestructibleSystem    _destructibleSystem;
    RealEngine::HealthSystem          _healthSystem;
    RealEngine::ParallaxSystem        _parallaxSystem;
    RealEngine::ParticleSystem        _particleSystem;
    RealEngine::NetvarSystem          _netvarSystem;
    rtype::Controls                   _controls;

    std::shared_ptr<RealEngine::Entity>                               _player_entity;
    std::unordered_map<long int, std::shared_ptr<RealEngine::Entity>> _players;
    std::unordered_map<long int, std::shared_ptr<RealEngine::Entity>> _entities;
    std::vector<std::shared_ptr<RealEngine::Entity>>                  _backgroundEntities;
    long int                                                          _localPlayerUUID;
    std::chrono::steady_clock::time_point                             _startTime;
    long                                                              _serverTime;
};
}  // namespace rtype

#endif /* !GAME_HPP_ */