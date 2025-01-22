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
    if (_sceneManager.getCurrentSceneType() == RealEngine::SceneType::WAITING) {
        const auto waitingRoomScene =
            std::dynamic_pointer_cast<WaitingRoomScene>(_sceneManager.getCurrentScene());
        if (waitingRoomScene) {
            waitingRoomScene->handleNewClient(sender);
        }
    }
}

void RtypeServer::initScenes() {
    _sceneManager.registerScene(RealEngine::SceneType::WAITING,
                                [this](RealEngine::Registry& registry) {
                                    return std::make_shared<WaitingRoomScene>(
                                        _gameInstance, this, _serverConfig, _server, _sceneManager);
                                });

    _sceneManager.changeScene(RealEngine::SceneType::WAITING, _gameInstance->getRegistryRef());

    _sceneManager.registerScene(
        RealEngine::SceneType::MENU, [this](RealEngine::Registry& registry) {
            return std::make_shared<MenuScene>(_gameInstance, this, _serverConfig, _server);
        });
    _sceneManager.registerScene(RealEngine::SceneType::GAME,
                                [this](RealEngine::Registry& registry) {
                                    return std::make_shared<GameScene>(
                                        _gameInstance, this, _serverConfig, _server, _sceneManager);
                                });
}