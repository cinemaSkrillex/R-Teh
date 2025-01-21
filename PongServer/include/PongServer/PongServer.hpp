/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** PongServer
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

#include "GameInstance/PongGameInstance.hpp"
#include "PongServerUtils.hpp"
#include "Server/UDPServer.hpp"
#include "Shared/PongServerProtocol.hpp"

#if defined(_WIN32) || defined(_WIN64)
#define _WIN32_WINNT 0x0A00
#endif

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

const float SNAP_TRESHOLD = 3.5f;

class PongServer {
   public:
    PongServer(std::shared_ptr<UDPServer> server, bool server_vision);
    ~PongServer();

    void                                  run();
    std::shared_ptr<UDPServer>            getServer() { return _server; }
    std::chrono::steady_clock::time_point getStartTime() { return _startTime; }

    void initCallbacks();
    void handleClientMessages();
    void broadcastStates();
    void broadcastAllReliable(const std::array<char, 800>& message);
    void broadcastAllUnreliable(const std::array<char, 800>& message);
    void handlePlayerDirection(const std::array<char, 800>&        message,
                               const asio::ip::udp::endpoint&      client,
                               std::shared_ptr<RealEngine::Entity> player);

   private:
    std::shared_ptr<UDPServer>            _server;
    float                                 _deltaTime;
    float                                 _deltaTimeBroadcast;
    int                                   _playerReadyCount;
    std::vector<long int>                 _readyPlayers;
    sf::Clock                             _clock;
    sf::Clock                             _broadcastClock;
    std::chrono::steady_clock::time_point _startTime;
    std::shared_ptr<PongGameInstance>     _gameInstance;
    std::shared_ptr<RealEngine::Entity>   _ball;
    std::shared_ptr<RealEngine::Entity>   _background;
    std::unordered_map<asio::ip::udp::endpoint, std::shared_ptr<RealEngine::Entity>> _players;
    // player timestamp
    std::unordered_map<asio::ip::udp::endpoint, long> _playerTimestamps;
    std::vector<asio::ip::udp::endpoint>              _spectators;
};
