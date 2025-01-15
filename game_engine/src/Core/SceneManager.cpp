/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** SceneManager
*/

#include "SceneManager.hpp"

#include <iostream>

#include "Registry.hpp"

RealEngine::SceneManager::SceneManager(RealEngine::Registry& registry)
    : _registry(registry), _currentScene(SceneType::MENU) {}

void RealEngine::SceneManager::switchScene(SceneType scene) {
    if (_sceneUpdateLogic.find(scene) != _sceneUpdateLogic.end()) {
        _currentScene = scene;
        std::cout << "Switched to scene: " << static_cast<int>(scene) << std::endl;
    } else {
        std::cerr << "Scene not registered!" << std::endl;
    }
}

void RealEngine::SceneManager::updateCurrentScene() {
    if (_sceneUpdateLogic.find(_currentScene) != _sceneUpdateLogic.end()) {
        _sceneUpdateLogic[_currentScene](_registry);
    } else {
        std::cerr << "No update logic for the current scene!" << std::endl;
    }
}

void RealEngine::SceneManager::registerScene(
    SceneType scene, std::function<void(RealEngine::Registry&)> updateLogic) {
    std::cout << "Registering scene: " << static_cast<int>(scene) << std::endl;
    _sceneUpdateLogic[scene] = updateLogic;
}
