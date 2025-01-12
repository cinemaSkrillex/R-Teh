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

#include "../Game/Block.hpp"
#include "Engine.hpp"
#include "RtypeServerProtocol.hpp"

namespace Map {
struct Tile {
    std::string  type;      // e.g., "BLOCK", "NONE"
    std::string  element;   // e.g., "background"
    sf::Vector2f position;  // e.g., (0, 0)
    float        rotation;  // e.g., (90.0)
};

struct WaveMob {
    std::string  name;       // Name of the mob (e.g., "eye_minion")
    sf::Vector2f position;   // Position of the mob
    float        angle = 0;  // Angle of the mob (if it mentionned)
};

struct Wave {
    std::string          waveType;       // e.g., "wave1" (filename of the wave JSON)
    sf::Vector2f         startPosition;  // Starting position for the wave
    std::vector<WaveMob> mobs;           // List of mobs in this wave
};
}  // namespace Map

class GameMap {
    // JSON helpers
    Json::Value readJSONFile(const std::string& filepath);
    void        writeJSONFile(const std::string& filepath, const Json::Value& json);

   public:
    GameMap(RealEngine::Registry& registry);
    ~GameMap();
    void updateLevel(float deltaTime);

    void loadFromJSON(const std::string& filepath);
    void saveToJSON(const std::string& filepath);

    // Accessor methods
    const std::vector<Map::Tile>&               getTiles() const { return _tiles; }
    const std::vector<Map::Wave>&               getWaves() const { return _waves; }
    bool                                        isLoaded() const { return _isLoaded; }
    std::vector<std::shared_ptr<rtype::Block>>& getBlockEntities() { return _blockEntities; }

    void removeBlockEntity(std::shared_ptr<rtype::Block>& block);

    void setXLevelPosition(float xLevelPosition) { x_level_position = xLevelPosition; }

    float getScrollingSpeed() const { return _scrollingSpeed; }
    float getXLevelPosition() const { return x_level_position; }
    void  addTile(const Map::Tile& tile) { _tiles.push_back(tile); }
    // Accessor methods (omitted here for brevity but same as before)
    // Modifiers and debug methods (omitted here for brevity)
   private:
    std::string                                      _map_name;
    float                                            _scrollingSpeed  = 0.0f;
    float                                            x_level_position = 1.0f;
    std::vector<Map::Tile>                           _tiles;
    std::vector<Map::Wave>                           _waves;
    std::vector<std::shared_ptr<rtype::Block>>       _blockEntities;
    std::vector<std::shared_ptr<RealEngine::Entity>> _WaveEntities;
    RealEngine::Registry&                            _registry;
    bool                                             _isLoaded = false;
    bool                                             _levelStarted;
};

#endif /* !GameMap_HPP_ */
