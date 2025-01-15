/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** SceneManager
*/

#ifndef SCENEMANAGER_HPP_
#define SCENEMANAGER_HPP_

#pragma once

#include <functional>
#include <unordered_map>

enum class SceneType { WAITING, MENU, GAME, PAUSE };

namespace RealEngine {
class Registry;

class SceneManager {
   public:
    SceneManager(RealEngine::Registry& registry);

    // Switches to the specified scene
    void switchScene(SceneType scene);

    // Updates the current scene
    void updateCurrentScene();

    // Registers a scene and its update logic
    void registerScene(SceneType scene, std::function<void(RealEngine::Registry&)> updateLogic);

   private:
    RealEngine::Registry& _registry;
    SceneType             _currentScene;

    // Map to hold scene-specific update logic
    std::unordered_map<SceneType, std::function<void(RealEngine::Registry&)>> _sceneUpdateLogic;
};
}  // namespace RealEngine
#endif /* !SCENEMANAGER_HPP_ */
