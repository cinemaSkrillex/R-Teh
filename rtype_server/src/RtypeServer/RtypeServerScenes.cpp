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
        std::cout << "Initializing GAME scene" << std::endl;
        startAndBroadcastLevel();
    });

    _scene_manager.registerScene(SceneType::PAUSE, [this](RealEngine::Registry& registry) {
        // Initialization logic for PAUSE scene
        std::cout << "Initializing PAUSE scene" << std::endl;
    });
}

void RtypeServer::updateScene() { _scene_manager.updateCurrentScene(); }