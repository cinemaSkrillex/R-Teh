/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** WaitingRoomScene
*/

#ifndef WAITINGROOMSCENE_HPP_
#define WAITINGROOMSCENE_HPP_

#include <asio/ip/udp.hpp>
#include <iostream>

#include "../../Game/GameInstance.hpp"
#include "MapInitializer.hpp"
#include "PlayerInitializer.hpp"
#include "Scene.hpp"
#include "ServerConfig.hpp"

class WaitingRoomScene : public Scene {
   public:
    WaitingRoomScene(std::shared_ptr<GameInstance> gameInstance, RtypeServer* server,
                     ServerConfig& serverConfig, std::shared_ptr<UDPServer> UdpServer)
        : _gameInstance(gameInstance),
          _server(server),
          _serverConfig(serverConfig),
          _UdpServer(UdpServer) {}

    void handleNewClient(const asio::ip::udp::endpoint& sender) {
        auto playerInitializer = std::make_shared<PlayerInitializer>(_server);
        std::cout << "New client connected: " << sender << std::endl;
        auto player = _server->getPlayer(sender);
        if (!player.getEntity()) {
            std::cout << "Player UUID is null" << std::endl;
            return;
        }
        playerInitializer->sendNewClientMessage(sender, player);
        playerInitializer->sendSynchronizeMessage(sender, player);
        initialize(_gameInstance->getRegistryRef());
    }

    void initialize(RealEngine::Registry& registry) override {
        auto gameMap = _gameInstance->getMap();
        gameMap->loadFromJSON("../../assets/maps/level_waiting.json");
        auto mapInitializer =
            std::make_shared<MapInitializer>(_gameInstance, _UdpServer, _serverConfig);
        _server->startAndBroadcastLevel();

        for (auto& client : _UdpServer->getClients()) {
            std::cout << "Yes i know New client connected: " << client << std::endl;
            mapInitializer->initializeMap(client);
        }
        std::cout << "Initializing WAITING scene" << std::endl;
    }

    void update(float deltaTime) override {}

    void render() override {}

   private:
    std::shared_ptr<GameInstance> _gameInstance;
    RtypeServer*                  _server;
    ServerConfig                  _serverConfig;
    std::shared_ptr<UDPServer>    _UdpServer;
};

#endif /* !WAITINGROOMSCENE_HPP_ */