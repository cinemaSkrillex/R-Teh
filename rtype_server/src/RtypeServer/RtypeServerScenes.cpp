/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** RtypeServerScenes
*/

#include "GameScene.hpp"
#include "MenuScene.hpp"
#include "RtypeServer.hpp"
#include "SceneManager.hpp"
#include "WaitingRoomScene.hpp"

void RtypeServer::notifyCurrentSceneOfNewClient(const asio::ip::udp::endpoint& sender) const {
    if (_scene_manager.getCurrentSceneType() == RealEngine::SceneType::WAITING) {
        const auto waitingRoomScene =
            std::dynamic_pointer_cast<WaitingRoomScene>(_scene_manager.getCurrentScene());
        if (waitingRoomScene) {
            waitingRoomScene->handleNewClient(sender);
        }
    }
}

void RtypeServer::initScenes() {
    _scene_manager.registerScene(
        RealEngine::SceneType::WAITING, [this](RealEngine::Registry& registry) {
            return std::make_shared<WaitingRoomScene>(_game_instance, this, _server_config, _server,
                                                      _scene_manager);
        });

    _scene_manager.changeScene(RealEngine::SceneType::WAITING, _game_instance->getRegistryRef());

    _scene_manager.registerScene(
        RealEngine::SceneType::MENU, [this](RealEngine::Registry& registry) {
            return std::make_shared<MenuScene>(_game_instance, this, _server_config, _server);
        });
    _scene_manager.registerScene(
        RealEngine::SceneType::GAME, [this](RealEngine::Registry& registry) {
            return std::make_shared<GameScene>(_game_instance, this, _server_config, _server,
                                               _scene_manager);
        });
}

// void RtypeServer::updateScene() { _scene_manager.updateCurrentScene(); }

// void RtypeServer::switchScene(SceneType scene) { _scene_manager.switchScene(scene); }