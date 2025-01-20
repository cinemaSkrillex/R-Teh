/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** GameMap
*/

#ifndef GameMap_HPP_
#define GameMap_HPP_

#include <json/json.h>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "Engine.hpp"

namespace rtype {
class Game;

class GameMap {
   public:
    GameMap(RealEngine::Registry& registry, Game* game);
    ~GameMap();
    void updateLevel(float deltaTime);
    void startLevel();
    void stopLevel();
    void unloadLevel();

    void addBlock(std::shared_ptr<RealEngine::Entity> block, long int blockId) {
        _blockEntities.emplace_back(std::make_pair(blockId, block));
    }
    void addBackground(std::shared_ptr<RealEngine::Entity> background,
                       RealEngine::ParallaxSystem&         parallaxSystem);
    void synchroniseLevelBlockEntities();
    void removeDeadBlocks();
    void setXLevelPosition(float xLevelPosition) { x_level_position = xLevelPosition; }
    void setScrollingSpeed(float scrollingSpeed) { _scrollingSpeed = scrollingSpeed; }
    void setIsMapLoaded(bool isMapLoaded) { _isMapLoaded = isMapLoaded; }
    void setLevelRunning(bool levelRunning) { _levelRunning = levelRunning; }
    void setMusicName(const std::string& musicName) {
        std::cout << "Setting music name: " << musicName << std::endl;
        _music_name = musicName;
        std::cout << "Music name set: " << _music_name << std::endl;
    }
    std::vector<std::pair<long int, std::shared_ptr<RealEngine::Entity>>>& getBlockEntities() {
        return _blockEntities;
    }
    std::vector<std::shared_ptr<RealEngine::Entity>> getBackgroundEntities() {
        return _backgroundEntities;
    }
    float       getScrollingSpeed() const { return _scrollingSpeed; }
    float       getXLevelPosition() const { return x_level_position; }
    bool        isMapLoaded() const { return _isMapLoaded; }
    bool        levelRunning() const { return _levelRunning; }
    std::string getMusicName() const { return _music_name; }

   private:
    RealEngine::Registry&                                                 _registry;
    Game*                                                                 _game;
    std::string                                                           _map_name;
    std::string                                                           _music_name      = "";
    float                                                                 _scrollingSpeed  = 0.0f;
    float                                                                 x_level_position = 0.0f;
    std::vector<std::pair<long int, std::shared_ptr<RealEngine::Entity>>> _blockEntities;
    std::vector<std::shared_ptr<RealEngine::Entity>>                      _backgroundEntities;
    bool                                                                  _levelRunning;
    bool                                                                  _isMapLoaded = false;
};
}  // namespace rtype

#endif /* !GameMap_HPP_ */
