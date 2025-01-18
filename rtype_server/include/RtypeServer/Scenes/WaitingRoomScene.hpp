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
          _scene_manager(scene_manager),
          _isChangingScene(false) {}

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

    static std::array<char, 800> createChangeSceneMessage(RTypeProtocol::SceneType sceneType) {
        RTypeProtocol::ChangingSceneMessage changeSceneMessage = {};
        changeSceneMessage.message_type                        = RTypeProtocol::CHANGING_SCENE;
        changeSceneMessage.scene_id                            = sceneType;
        return RTypeProtocol::serialize<800>(changeSceneMessage);
    }

    void update(float deltaTime) override {
        if (_isChangingScene) {
            std::cerr << "WAITING : Scene is changing" << std::endl;
            return;
        }

        auto map    = _gameInstance->getMap();
        auto blocks = map->getBlockEntities();
        // find all the WaitingBlock entities and send them to the new client
        for (const auto& block : blocks) {
            auto waitingBlock = std::dynamic_pointer_cast<rtype::WaitingBlock>(block);
            if (waitingBlock) {
                auto waitingBlockEntity = *waitingBlock->getEntity();
                // std::cout << "WaitingBlock entity: " << waitingBlockEntity << std::endl;
                int  playerInBox = waitingBlock->getPlayersInBox();
                auto container =
                    _gameInstance->getRegistryRef().get_component<RealEngine::NetvarContainer>(
                        waitingBlockEntity);

                if (!container) {
                    std::cout
                        << "Error: WaitingBlock entity does not have a NetvarContainer component"
                        << std::endl;
                    return;
                }
                auto netvar = container->getNetvar("change_scene_timer");
                if (!netvar) {
                    std::cout << "Error: WaitingBlock entity does not have a change_scene_timer"
                                 " netvar"
                              << std::endl;
                    return;
                }
                float timer = std::any_cast<float>(netvar->value);
                if (playerInBox > 0 &&
                    playerInBox == static_cast<int>(_UdpServer->getClients().size())) {
                    timer -= deltaTime;
                    if (timer <= 0) {
                        _isChangingScene = true;
                        std::cout << "All players are ready" << std::endl;
                        _scene_manager.changeScene(RealEngine::SceneType::GAME,
                                                   _gameInstance->getRegistryRef());
                        auto changeSceneMessage =
                            createChangeSceneMessage(RTypeProtocol::SceneType::GAME);
                        for (const auto& client : _UdpServer->getClients()) {
                            _UdpServer->send_reliable_packet(changeSceneMessage, client);
                        }
                        _gameInstance->getMap()->unloadLevel();
                    }
                    if (!container->getNetvar("change_scene_timer")) {
                        std::cout << "Error: WaitingBlock entity does not have a change_scene_timer"
                                     " netvar"
                                  << std::endl;
                        return;
                    }
                    netvar->value = timer;
                    // std::cout << "IN PLAYER BOXTimer: " << timer << std::endl;
                    // std::cout <<
                    // "std::any_cast<float>(container->getNetvar(\"change_scene_timer\")"
                    //              "->value): "
                    //           << std::any_cast<float>(
                    //                  container->getNetvar("change_scene_timer")->value)
                    //           << std::endl;

                } else if (playerInBox < static_cast<int>(_UdpServer->getClients().size())) {
                    container->getNetvar("change_scene_timer")->value = 1.0f;
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
    bool                          _isChangingScene;
};

#endif /* !WAITINGROOMSCENE_HPP_ */