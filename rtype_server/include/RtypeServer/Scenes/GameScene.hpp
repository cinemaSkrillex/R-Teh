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

// _scene_manager.registerScene(
//     RealEngine::SceneType::GAME, [this](RealEngine::Registry& registry) {
//         // Initialization logic for GAME scene
//         // works but right now we don't use it
//         std::cout << "Initializing GAME scene" << std::endl;
//         auto        gameMap = _game_instance->getMap();
//         std::string path    = "../../assets/maps/";
//         gameMap->loadFromJSON(path + "level_1.json");
//         auto mapInitializer =
//             std::make_shared<MapInitializer>(_game_instance, _server, _server_config);
//         auto mobInitializer = std::make_shared<MobInitializer>(_game_instance, _server);

//         startAndBroadcastLevel();
//         for (auto& client : _server->getClients()) {
//             mapInitializer->initializeMap(client);
//             mobInitializer->initializeMobs(client);
//         }
//     });

class GameScene : public Scene {
   public:
    GameScene(std::shared_ptr<GameInstance> gameInstance, RtypeServer* server,
              ServerConfig& serverConfig, std::shared_ptr<UDPServer> udpServer)
        : _gameInstance(gameInstance),
          _server(server),
          _serverConfig(serverConfig),
          _UdpServer(udpServer) {
        initialize(_gameInstance->getRegistryRef());
    }

    void initialize(RealEngine::Registry& registry) override {
        // Initialization logic for MENU scene
        std::cout << "Initializing GAME scene" << std::endl;
    }

    void update(float deltaTime) override {
        // Update logic for MENU scene
    }

    void render() override {
        // Render logic for MENU scene
    }

   private:
    std::shared_ptr<GameInstance> _gameInstance;
    RtypeServer*                  _server;
    ServerConfig                  _serverConfig;
    std::shared_ptr<UDPServer>    _UdpServer;
};

#endif /* !GAMESCENE_HPP_ */
