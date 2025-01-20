#ifndef SCENEMANAGER_HPP_
#define SCENEMANAGER_HPP_

#include <functional>
#include <iostream>
#include <memory>
#include <unordered_map>

#include "Scene.hpp"

namespace RealEngine {

enum class SceneType { WAITING, MENU, GAME };

class SceneManager {
   public:
    void registerScene(SceneType                                                    type,
                       std::function<std::shared_ptr<Scene>(RealEngine::Registry&)> createScene) {
        _sceneFactories[type] = createScene;
    }

    void changeScene(SceneType type, RealEngine::Registry& registry) {
        auto it = _sceneFactories.find(type);
        if (it != _sceneFactories.end()) {
            _currentScene     = it->second(registry);
            _currentSceneType = type;
            std::cout << "Changing scene to " << static_cast<int>(type) << std::endl;
        }
    }

    std::shared_ptr<Scene> getCurrentScene() const { return _currentScene; }

    SceneType getCurrentSceneType() const { return _currentSceneType; }

   private:
    std::unordered_map<SceneType, std::function<std::shared_ptr<Scene>(RealEngine::Registry&)>>
                           _sceneFactories;
    std::shared_ptr<Scene> _currentScene;
    SceneType              _currentSceneType;
};

}  // namespace RealEngine

#endif /* SCENEMANAGER_HPP_ */