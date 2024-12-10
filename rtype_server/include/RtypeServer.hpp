/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RtypeServer
*/

#pragma once

#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <asio.hpp>
#include <chrono>
#include <iostream>
#include <thread>

#include "Game/GameInstance.hpp"
#include "Server/UDPServer.hpp"

#if defined(_WIN32) || defined(_WIN64)
#define _WIN32_WINNT 0x0A00
#endif

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

const sf::Int32    SERVER_TICK           = 10;
const sf::Vector2f PLAYER_START_POSITION = {50.f, 250.f};

class Player {
   public:
    long int     getUUID() const { return _uuid; }
    sf::Vector2f getPosition() const {
        RealEngine::Position* component = _registry->get_component<RealEngine::Position>(*_entity);
        return {component->x, component->y};
    }
    const std::vector<sf::Vector2f>& getPositions() const { return _positions; }
    void     setLastTimestamp(long int timestamp) { _last_update = timestamp; }
    long int getLastTimestamp() const { return _last_update; }
    void     setPositionByUuid(long int uuid, sf::Vector2f position);

   private:
    long int                  _uuid;
    std::vector<sf::Vector2f> _positions;
    RealEngine::Entity*       _entity;
    RealEngine::Registry*     _registry;
    long                      _last_update;

   public:
    Player(/* args */);
    Player(long int uuid, long int timestamp, RealEngine::Entity* player_entity,
           RealEngine::Registry* registry);
    ~Player();
};

static std::vector<Player> PLAYERS = {};

class RtypeServer {
   private:
    std::shared_ptr<UDPServer>                          _server;
    std::shared_ptr<GameInstance>                       _game_instance;
    std::unordered_map<asio::ip::udp::endpoint, Player> _players;
    float                                               _deltaTime;
    sf::Clock                                           _clock;
    sf::Clock                                           _broadcastClock;
    std::chrono::steady_clock::time_point               _startTime;

   public:
    RtypeServer(std::shared_ptr<UDPServer> server);
    ~RtypeServer();

    void run();

    std::shared_ptr<GameInstance>         getGameInstance() { return _game_instance; }
    std::shared_ptr<UDPServer>            getServer() { return _server; }
    std::chrono::steady_clock::time_point getStartTime() { return _startTime; }
};
