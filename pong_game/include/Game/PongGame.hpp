/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** PongGame.hpp
*/

#ifndef PONGGAME_HPP_
#define PONGGAME_HPP_

#define GAME_SCALE 2.0f

#include <SFML/Graphics.hpp>
#include <chrono>
#include <regex>

#include "Client/UDPClient.hpp"
#include "Components/Score.hpp"
#include "Core/AssetsManager.hpp"
#include "Core/View.hpp"
#include "Core/Window.hpp"
#include "ECS/Registry/Registry.hpp"
#include "PongControls.hpp"
#include "Systems/CollisionSystem.hpp"
#include "Systems/ControlSystem.hpp"
#include "Systems/DrawSystem.hpp"
#include "Systems/LagCompensationSystem.hpp"
#include "Systems/MovementSystem.hpp"
#include "Systems/NetvarSystem.hpp"
#include "Systems/ParallaxSystem.hpp"
#include "Systems/ParticleSystem.hpp"
#include "shared/PongServerProtocol.hpp"

namespace pong {
class Game {
   public:
    Game(std::shared_ptr<UDPClient> clientUDP, unsigned short client_port);
    ~Game();

    void        run();
    void        handleSignal(std::array<char, 800> signal);
    sf::IntRect getPlayerNormalizedDirection();

    std::shared_ptr<UDPClient> _clientUDP;

   private:
    void init_systems();
    void init_textures();
    void init_musics();
    void init_sounds();
    void init_fonts();
    void init_screen_limits();

    void register_components();
    void bind_keys();
    void set_action_handlers();

    // GameSignals
    void handlePlayerMove(PongProtocol::PlayerMoveMessage parsedPacket);
    void handleNewEntity(PongProtocol::NewEntityMessage parsedPacket);
    void handleEntityUpdate(PongProtocol::EntityUpdateMessage parsedPacket);
    void handlePlayerValues(PongProtocol::PlayerUpdateDataMessage parsedPacket);

    float              _deltaTime = 0.f;
    RealEngine::View   _view;
    RealEngine::Window _window;
    sf::Clock          _clock;
    sf::Clock          _broadcastClock;
    int                _serverTick;

    RealEngine::Registry              _registry;
    RealEngine::LagCompensationSystem _lagCompensationSystem;
    RealEngine::DrawSystem            _drawSystem;
    RealEngine::ControlSystem         _controlSystem;
    RealEngine::MovementSystem        _movementSystem;
    RealEngine::ParticleSystem        _particleSystem;
    RealEngine::NetvarSystem          _netvarSystem;
    pong::Controls                    _controls;

    std::shared_ptr<RealEngine::Entity>                               _player_entity;
    std::unordered_map<long int, std::shared_ptr<RealEngine::Entity>> _entities;

    std::chrono::steady_clock::time_point _startTime;
    std::chrono::steady_clock::time_point _lastTimestamp;
};
}  // namespace pong

#endif /* !PONGGAME_HPP_ */