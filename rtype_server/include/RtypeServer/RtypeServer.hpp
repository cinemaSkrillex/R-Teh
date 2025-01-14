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
#include <regex>
#include <thread>
#include <unordered_map>
#include <vector>

#include "Events/IEvent.hpp"
#include "Game/Block.hpp"
#include "Game/GameInstance.hpp"
#include "GenerateUuid.hpp"
#include "HoldShootEvent.hpp"
#include "Log.hpp"
#include "PlayerUtils.hpp"
#include "ReleaseShootEvent.hpp"
#include "RtypeServer/Callbacks/MapInitializer.hpp"
#include "RtypeServer/Callbacks/MobInitializer.hpp"
#include "RtypeServer/Callbacks/PlayerInitializer.hpp"
#include "RtypeServer/RtypeServerUtils.hpp"
#include "RtypeServerProtocol.hpp"
#include "Server/UDPServer.hpp"
#include "ServerConfig.hpp"
#include "ShootEvent.hpp"

#if defined(_WIN32) || defined(_WIN64)
#define _WIN32_WINNT 0x0A00
#endif

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

const float SNAP_TRESHOLD = 3.5f;

class Player {
   public:
    long int     getUUID() const { return _uuid; }
    sf::Vector2f getPosition() const {
        RealEngine::Position* component = _registry->get_component<RealEngine::Position>(*_entity);
        if (!component) {
            std::cout << "Error: Player entity does not have a Position component" << std::endl;
            return {0, 0};
        }
        return {component->x, component->y};
    }
    const std::vector<sf::Vector2f>& getPositions() const { return _positions; }
    void     setLastTimestamp(long int timestamp) { _last_update = timestamp; }
    long int getLastTimestamp() const { return _last_update; }
    void     setPositionByUuid(long int uuid, sf::Vector2f position);
    std::shared_ptr<RealEngine::Entity> getEntity() { return _entity; }
    std::shared_ptr<RealEngine::Entity> getEntity() const { return _entity; }

   private:
    long int                            _uuid;
    std::vector<sf::Vector2f>           _positions;
    std::shared_ptr<RealEngine::Entity> _entity;
    RealEngine::Registry*               _registry;
    long                                _last_update;

   public:
    Player(/* args */);
    Player(long int uuid, long int timestamp, std::shared_ptr<RealEngine::Entity> _player_entity,
           RealEngine::Registry* registry);
    ~Player();
};

static std::vector<Player> PLAYERS = {};

class RtypeServer {
   private:
    std::shared_ptr<UDPServer>                          _server;
    std::shared_ptr<GameInstance>                       _game_instance;
    ServerConfig                                        _server_config;
    std::unordered_map<asio::ip::udp::endpoint, Player> _players;
    float                                               _deltaTime;
    float                                               _deltaTimeBroadcast;
    sf::Clock                                           _clock;
    sf::Clock                                           _broadcastClock;
    std::chrono::steady_clock::time_point               _startTime;
    std::unordered_map<int, std::unique_ptr<IEvent>>    eventHandlers;

    void initCallbacks();
    void initEventHandlers();
    void startAndBroadcastLevel();

    void broadcastPlayerState(const Player& player);
    void broadcastStartLevel();
    void broadcastEntityState(int uuid, const std::shared_ptr<RealEngine::Entity> entity);
    void broadcastAllReliable(const std::array<char, 800>& message);
    void broadcastAllUnreliable(const std::array<char, 800>& message);

    std::string formatTimestamp(const std::chrono::steady_clock::time_point& timestamp);

    void handleClientMessages();
    void runGameInstance(float deltaTime);
    void broadcastStates();

    void sendNewEntity(RealEngine::Entity entity, RealEngine::Registry* registry);

    void printServerStartupBanner();

   public:
    RtypeServer(std::shared_ptr<UDPServer> server, bool server_vision);
    ~RtypeServer();

    void run();
    void runEvent(const std::array<char, 800>& buffer, const asio::ip::udp::endpoint& client,
                  Player& player);
    void runSimulation(const std::array<char, 800>& buffer, const asio::ip::udp::endpoint& client,
                       Player& player);
    ServerConfig                                        getServerConfig() { return _server_config; }
    std::unordered_map<asio::ip::udp::endpoint, Player> getPlayers() { return _players; }
    std::shared_ptr<GameInstance>                       getGameInstance() { return _game_instance; }
    std::shared_ptr<UDPServer>                          getServer() { return _server; }
    std::chrono::steady_clock::time_point               getStartTime() { return _startTime; }
};
