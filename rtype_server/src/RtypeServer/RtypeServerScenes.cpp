/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** RtypeServerScenes
*/

#include "RtypeServer.hpp"

void RtypeServer::initScenes() {
    _scene_manager.registerScene(SceneType::WAITING, [this](RealEngine::Registry& registry) {
        // Initialization logic for WAITING scene
        std::cout << "Initializing WAITING scene" << std::endl;
    });

    _scene_manager.registerScene(SceneType::MENU, [this](RealEngine::Registry& registry) {
        // Initialization logic for MENU scene
        std::cout << "Initializing MENU scene" << std::endl;
    });

    _scene_manager.registerScene(SceneType::GAME, [this](RealEngine::Registry& registry) {
        // Initialization logic for GAME scene
        // works but right now we don't use it
        std::cout << "Initializing GAME scene" << std::endl;
        auto        gameMap = _game_instance->getMap();
        std::string path    = "../../assets/maps/";
        gameMap->loadFromJSON(path + "level_1.json");
        auto mapInitializer =
            std::make_shared<MapInitializer>(_game_instance, _server, _server_config);
        auto mobInitializer = std::make_shared<MobInitializer>(_game_instance, _server);

        startAndBroadcastLevel();
        for (auto& client : _server->getClients()) {
            mapInitializer->initializeMap(client);
            mobInitializer->initializeMobs(client);
        }
    });

    _scene_manager.registerScene(SceneType::PAUSE, [this](RealEngine::Registry& registry) {
        // Initialization logic for PAUSE scene
        std::cout << "Initializing PAUSE scene" << std::endl;
    });
}

void RtypeServer::updateScene() { _scene_manager.updateCurrentScene(); }

void RtypeServer::switchScene(SceneType scene) { _scene_manager.switchScene(scene); }