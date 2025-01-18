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
#include <utility>

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

    void addBlock(std::shared_ptr<RealEngine::Entity> block, const long int blockId) {
        // std::cout << "Adding block to game map" << "uuid: " << blockId << std::endl;
        // std::cout << "Block ID entity: " << *block << std::endl;
        _blockEntities[blockId] = std::move(block);
    }
    void addBackground(std::shared_ptr<RealEngine::Entity> background,
                       RealEngine::ParallaxSystem&         parallaxSystem);
    void synchroniseLevelBlockEntities();
    void removeDeadBlocks();
    void setXLevelPosition(float xLevelPosition) { x_level_position = xLevelPosition; }
    void setScrollingSpeed(float scrollingSpeed) { _scrollingSpeed = scrollingSpeed; }
    void setIsMapLoaded(bool isMapLoaded) { _isMapLoaded = isMapLoaded; }
    void setMusicName(const std::string& musicName) {
        std::cout << "Setting music name: " << musicName << std::endl;
        _music_name = musicName;
        std::cout << "Music name set: " << _music_name << std::endl;
    }
    std::unordered_map<long int, std::shared_ptr<RealEngine::Entity>>& getBlockEntities() {
        return _blockEntities;
    }
    float getScrollingSpeed() const { return _scrollingSpeed; }
    float getXLevelPosition() const { return x_level_position; }
    bool  isMapLoaded() const { return _isMapLoaded; }
    bool  levelRunning() const { return _levelRunning; }

   private:
    RealEngine::Registry&                                             _registry;
    Game*                                                             _game;
    std::string                                                       _map_name;
    std::string                                                       _music_name      = "";
    float                                                             _scrollingSpeed  = 0.0f;
    float                                                             x_level_position = 0.0f;
    std::unordered_map<long int, std::shared_ptr<RealEngine::Entity>> _blockEntities;
    std::vector<std::shared_ptr<RealEngine::Entity>>                  _backgroundEntities;
    bool                                                              _levelRunning;
    bool                                                              _isMapLoaded = false;
};
}  // namespace rtype

#endif /* !GameMap_HPP_ */
