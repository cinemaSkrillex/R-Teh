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

class GameMap {
   public:
    GameMap(RealEngine::Registry& registry);
    ~GameMap();
    void updateLevel(float deltaTime);

    void addBlock(std::shared_ptr<RealEngine::Entity> block) { _blockEntities.push_back(block); }
    void removeDeadBlocks();

    void setXLevelPosition(float xLevelPosition) { x_level_position = xLevelPosition; }

    std::vector<std::shared_ptr<RealEngine::Entity>>& getBlockEntities() { return _blockEntities; }
    float getScrollingSpeed() const { return _scrollingSpeed; }
    float getXLevelPosition() const { return x_level_position; }

   private:
    RealEngine::Registry&                            _registry;
    std::string                                      _map_name;
    float                                            _scrollingSpeed  = 0.0f;
    float                                            x_level_position = 0.0f;
    std::vector<std::shared_ptr<RealEngine::Entity>> _blockEntities;
    bool                                             _levelStarted;
};

#endif /* !GameMap_HPP_ */
