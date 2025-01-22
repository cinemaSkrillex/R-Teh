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
#include "Game/GameInstance.hpp"
#include "Game/Level/Block.hpp"
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
#include "SceneManager.hpp"
#include "Server/UDPServer.hpp"
#include "ServerConfig.hpp"
#include "ServerPlayer.hpp"
#include "ShootEvent.hpp"

#if defined(_WIN32) || defined(_WIN64)
#define _WIN32_WINNT 0x0A00
#endif

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

const float SNAP_TRESHOLD = 3.5f;

class RtypeServer {
   public:
    RtypeServer(std::shared_ptr<UDPServer> server, bool server_vision);
    ~RtypeServer();

    void run();
    void runEvent(const std::array<char, 800>& buffer, const asio::ip::udp::endpoint& client,
                  ServerPlayer& player);
    void runSimulation(const std::array<char, 800>& buffer, const asio::ip::udp::endpoint& client,
                       ServerPlayer& player);
    void broadcastStartLevel();
    void broadcastStopLevel();
    void startLevel();
    void stopLevel();
    bool allClientsUnloadedMap() const;

    ServerConfig getServerConfig() { return _serverConfig; }
    std::unordered_map<asio::ip::udp::endpoint, ServerPlayer> getPlayers() { return _players; }
    ServerPlayer& getPlayer(const asio::ip::udp::endpoint& client) { return _players[client]; }
    std::shared_ptr<GameInstance>         getGameInstance() { return _gameInstance; }
    std::shared_ptr<UDPServer>            getServer() { return _server; }
    std::chrono::steady_clock::time_point getStartTime() { return _startTime; }

   private:
    std::shared_ptr<UDPServer>                                               _server;
    std::shared_ptr<GameInstance>                                            _gameInstance;
    ServerConfig                                                             _serverConfig;
    RealEngine::SceneManager                                                 _sceneManager;
    std::unordered_map<asio::ip::udp::endpoint, ServerPlayer>                _players;
    float                                                                    _deltaTime;
    float                                                                    _deltaTimeBroadcast;
    sf::Clock                                                                _clock;
    sf::Clock                                                                _broadcastClock;
    std::chrono::steady_clock::time_point                                    _startTime;
    std::unordered_map<int, std::unique_ptr<IEvent>>                         _eventHandlers;
    std::unordered_set<asio::ip::udp::endpoint, EndpointHash, EndpointEqual> _clientsUnloadedMap;
    std::shared_ptr<Log>                                                     _log;

    void initCallbacks();
    void initEventHandlers();
    void initScenes();
    void startAndBroadcastLevel();

    void broadcastPlayerState(const ServerPlayer& player);
    void broadcastStartLevelUtils();
    void broadcastEntityState(RealEngine::Entity entity, RealEngine::Registry* registry);
    void broadcastAllReliable(const std::array<char, 800>& message);
    void broadcastAllUnreliable(const std::array<char, 800>& message);
    void notifyCurrentSceneOfNewClient(const asio::ip::udp::endpoint& sender) const;

    std::string formatTimestamp(const std::chrono::steady_clock::time_point& timestamp);

    void handleClientMessages();
    void handleMapUnloadedMessage(const asio::ip::udp::endpoint& client);

    void runGameInstance(float deltaTime);
    void broadcastStates();

    void sendNewEntity(RealEngine::Entity entity, RealEngine::Registry* registry);

    void printServerStartupBanner();
};
