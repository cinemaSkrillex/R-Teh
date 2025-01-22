/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** GameScene
*/

#ifndef GAMESCENE_HPP_
#define GAMESCENE_HPP_

#include <iostream>
#include <memory>

#include "../../Game/GameInstance.hpp"
#include "MapInitializer.hpp"
#include "PlayerInitializer.hpp"
#include "Scene.hpp"
#include "ServerConfig.hpp"

class GameScene : public Scene {
   public:
    GameScene(const std::shared_ptr<GameInstance>& gameInstance, RtypeServer* server,
              const ServerConfig& serverConfig, const std::shared_ptr<UDPServer>& udpServer,
              RealEngine::SceneManager& scene_manager)
        : _gameInstance(gameInstance),
          _server(server),
          _serverConfig(serverConfig),
          _UdpServer(udpServer),
          _isInitialized(false) {}

    void initialize(RealEngine::Registry& registry) override {
        std::cout << "Initializing GAME scene" << std::endl;
        const auto        gameMap = _gameInstance->getMap();
        const std::string path    = "../../assets/maps/";
        gameMap->loadFromJSON(path + "level_1.json");
        auto mapInitializer =
            std::make_shared<MapInitializer>(_gameInstance, _UdpServer, _serverConfig);

        _gameInstance->startLevel();
        _server->broadcastStartLevel();

        for (auto& client : _UdpServer->getClients()) {
            std::cout << "GAME yes i know New client connected: " << client << std::endl;
            mapInitializer->initializeMap(client);
        }
    }

    void update(float deltaTime) override {
        if (!_isInitialized && _server->allClientsUnloadedMap()) {
            initialize(_gameInstance->getRegistryRef());
            _isInitialized = true;
        }
    }

    void render() override {
        // Render logic for MENU scene
    }

   private:
    std::shared_ptr<GameInstance> _gameInstance;
    RtypeServer*                  _server;
    ServerConfig                  _serverConfig;
    std::shared_ptr<UDPServer>    _UdpServer;
    bool                          _isInitialized;
};

#endif /* !GAMESCENE_HPP_ */
