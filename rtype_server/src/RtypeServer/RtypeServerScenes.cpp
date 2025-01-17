/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** RtypeServerScenes
*/

#include "MenuScene.hpp"
#include "RtypeServer.hpp"
#include "WaitingRoomScene.hpp"

void RtypeServer::notifyCurrentSceneOfNewClient(const asio::ip::udp::endpoint& sender) {
    if (_scene_manager.getCurrentSceneType() == RealEngine::SceneType::WAITING) {
        auto waitingRoomScene =
            std::dynamic_pointer_cast<WaitingRoomScene>(_scene_manager.getCurrentScene());
        if (waitingRoomScene) {
            waitingRoomScene->handleNewClient(sender);
        }
        auto map    = _game_instance->getMap();
        auto blocks = map->getBlockEntities();
        // find all the WaitingBlock entities and send them to the new client
        for (const auto& block : blocks) {
            auto waitingBlock = std::dynamic_pointer_cast<rtype::WaitingBlock>(block);
            if (waitingBlock) {
                // Send the waitingBlock to the new client
                std::cout << "Found WaitingBlock entity: " << waitingBlock->getEntity()
                          << std::endl;
                // Add your logic to send the waitingBlock to the new client
            }
        }
    }
}

void RtypeServer::initScenes() {
    _scene_manager.registerScene(RealEngine::SceneType::WAITING,
                                 [this](RealEngine::Registry& registry) {
                                     return std::make_shared<WaitingRoomScene>(
                                         _game_instance, this, _server_config, _server);
                                 });

    _scene_manager.changeScene(RealEngine::SceneType::WAITING, _game_instance->getRegistryRef());

    _scene_manager.registerScene(
        RealEngine::SceneType::MENU, [this](RealEngine::Registry& registry) {
            return std::make_shared<MenuScene>(_game_instance, this, _server_config, _server);
        });
    // _scene_manager.registerScene(SceneType::WAITING, [this](RealEngine::Registry& registry) {
    //     // Initialization logic for WAITING scene
    //     auto gameMap = _game_instance->getMap();
    //     gameMap->loadFromJSON("../../assets/maps/level_waiting.json");
    //     auto mapInitializer =
    //         std::make_shared<MapInitializer>(_game_instance, _server, _server_config);
    //     startAndBroadcastLevel();
    //     auto playerInitializer = std::make_shared<PlayerInitializer>(this);
    //     for (auto& client : _server->getClients()) {
    //         std::cout << "New client connected: " << client << std::endl;
    //         for (const auto& player : _players) {
    //             std::cout << "Player UUID: " << player.first << std::endl;
    //             if (player.second.getEntity()) {
    //                 std::cout << "Player already initialized" << "player uuid :["
    //                           << player.second.getEntity() << "]" << std::endl;
    //                 continue;
    //             }
    //             playerInitializer->sendNewClientMessage(client, player.second);
    //             playerInitializer->sendSynchronizeMessage(client, player.second);
    //             std::cout << "Player initialized" << "player uuid :[" <<
    //             player.second.getEntity()
    //                       << "]" << std::endl;
    //         }
    //         mapInitializer->initializeMap(client);
    //     }
    //     std::cout << "Initializing WAITING scene" << std::endl;
    // });
    // _scene_manager.registerScene(SceneType::WAITING, [this](RealEngine::Registry& registry) {
    //     auto waitingRoomScene =
    //         std::make_shared<WaitingRoomScene>(_game_instance, _server, _server_config);
    //     waitingRoomScene->initialize(registry);
    //     _scene_manager.setCurrentScene(waitingRoomScene);
    // });

    // _scene_manager.changeScene(SceneType::WAITING);

    // _scene_manager.registerScene(SceneType::MENU, [this](RealEngine::Registry& registry) {
    //     // Initialization logic for MENU scene
    //     std::cout << "Initializing MENU scene" << std::endl;
    // });

    // _scene_manager.registerScene(SceneType::GAME, [this](RealEngine::Registry& registry) {
    //     // Initialization logic for GAME scene
    //     // works but right now we don't use it
    //     std::cout << "Initializing GAME scene" << std::endl;
    //     auto        gameMap = _game_instance->getMap();
    //     std::string path    = "../../assets/maps/";
    //     gameMap->loadFromJSON(path + "level_1.json");
    //     auto mapInitializer =
    //         std::make_shared<MapInitializer>(_game_instance, _server, _server_config);
    //     auto mobInitializer = std::make_shared<MobInitializer>(_game_instance, _server);

    //     startAndBroadcastLevel();
    //     for (auto& client : _server->getClients()) {
    //         mapInitializer->initializeMap(client);
    //         mobInitializer->initializeMobs(client);
    //     }
    // });

    // _scene_manager.registerScene(SceneType::PAUSE, [this](RealEngine::Registry& registry) {
    //     // Initialization logic for PAUSE scene
    //     std::cout << "Initializing PAUSE scene" << std::endl;
    // });
}

// void RtypeServer::updateScene() { _scene_manager.updateCurrentScene(); }

// void RtypeServer::switchScene(SceneType scene) { _scene_manager.switchScene(scene); }