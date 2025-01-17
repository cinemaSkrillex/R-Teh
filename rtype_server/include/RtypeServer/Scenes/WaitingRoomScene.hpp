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
                     ServerConfig& serverConfig, std::shared_ptr<UDPServer> UdpServer,
                     RealEngine::SceneManager& scene_manager)
        : _gameInstance(gameInstance),
          _server(server),
          _serverConfig(serverConfig),
          _UdpServer(UdpServer),
          _scene_manager(scene_manager) {}

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

    void update(float deltaTime) override {
        auto map    = _gameInstance->getMap();
        auto blocks = map->getBlockEntities();
        // find all the WaitingBlock entities and send them to the new client
        for (const auto& block : blocks) {
            auto waitingBlock = std::dynamic_pointer_cast<rtype::WaitingBlock>(block);
            if (waitingBlock) {
                auto waitingBlockEntity = *waitingBlock->getEntity();
                int  playerInBox        = waitingBlock->getPlayersInBox();
                auto container =
                    _gameInstance->getRegistryRef().get_component<RealEngine::NetvarContainer>(
                        waitingBlockEntity);

                float timer =
                    std::any_cast<float>(container->getNetvar("change_scene_timer")->value);

                if (playerInBox > 0 && playerInBox == _UdpServer->getClients().size()) {
                    timer -= deltaTime;
                    if (timer <= 0) {
                        std::cout << "All players are ready" << std::endl;
                        _scene_manager.changeScene(RealEngine::SceneType::GAME,
                                                   _gameInstance->getRegistryRef());
                    }
                    container->getNetvar("change_scene_timer")->value = timer;
                    std::cout << "IN PLAYER BOXTimer: " << timer << std::endl;
                    std::cout << "std::any_cast<float>(container->getNetvar(\"change_scene_timer\")"
                                 "->value): "
                              << std::any_cast<float>(
                                     container->getNetvar("change_scene_timer")->value)
                              << std::endl;

                } else if (playerInBox < _UdpServer->getClients().size()) {
                    container->getNetvar("change_scene_timer")->value = 3.0f;
                }
            }
            waitingBlock->ReinitPlayersInBox();
        }
    }

    void render() override {
        // Render logic for WAITING scene
    }

   private:
    std::shared_ptr<GameInstance> _gameInstance;
    RtypeServer*                  _server;
    ServerConfig                  _serverConfig;
    std::shared_ptr<UDPServer>    _UdpServer;
    RealEngine::SceneManager&     _scene_manager;
};

#endif /* !WAITINGROOMSCENE_HPP_ */