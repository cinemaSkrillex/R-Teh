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
