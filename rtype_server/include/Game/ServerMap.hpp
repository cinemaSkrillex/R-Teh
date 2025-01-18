/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** ServerMap
*/

#ifndef ServerMap_HPP_
#define ServerMap_HPP_

#include <json/json.h>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "../Game/Level/BaseBlock.hpp"
#include "../Game/Level/Block.hpp"
#include "../Game/Level/WaitingBlock.hpp"
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

struct Boss {
    std::string                         bossType   = "";  // e.g., "eye_boss" make the entity spawn
    sf::Vector2f                        position   = {-1, -1};  // Starting position for the boss
    std::shared_ptr<RealEngine::Entity> bossEntity = nullptr;
    bool                                triggered  = false;
};

}  // namespace Map

class ServerMap {
    // JSON helpers
    Json::Value readJSONFile(const std::string& filepath);
    void        writeJSONFile(const std::string& filepath, const Json::Value& json);

   public:
    ServerMap(RealEngine::Registry& registry);
    ~ServerMap();
    void                      updateLevel(float deltaTime);
    std::vector<Map::WaveMob> invokeLevelMobs();
    bool bossAtEnd() const { return _boss.position.x != -1 && _boss.position.y != -1; }
    void setBossEntity(std::shared_ptr<RealEngine::Entity> bossEntity) {
        _boss.bossEntity = bossEntity;
    }
    void startLevel();
    void stopLevel();

    void loadFromJSON(const std::string& filepath);
    void saveToJSON(const std::string& filepath);

    bool isLoaded() const { return _isLoaded; }
    void addTile(const Map::Tile& tile) { _tiles.push_back(tile); }
    void removeDeadBlocks();

    void unloadLevel();
    void setXLevelPosition(float xLevelPosition) { x_level_position = xLevelPosition; }

    std::vector<std::shared_ptr<rtype::BaseBlock>>& getBlockEntities() { return _blockEntities; }
    const std::vector<Map::Tile>&                   getTiles() const { return _tiles; }
    const std::vector<Map::Wave>&                   getWaves() const { return _waves; }
    std::string                                     getMapName() const { return _map_name; }
    std::string                                     getMusicName() const { return _music_name; }
    std::vector<std::pair<std::string, float>>      getBackgrounds() const { return _backgrounds; }

    float getScrollingSpeed() const { return _scrollingSpeed; }
    float getXLevelPosition() const { return x_level_position; }
    bool  getIsLevelRunning() const { return _isLevelRunning; }

   private:
    RealEngine::Registry&                      _registry;
    std::string                                _map_name;
    std::string                                _music_name;
    float                                      _scrollingSpeed  = 0.0f;
    float                                      x_level_position = 0.0f;
    std::vector<Map::Tile>                     _tiles;
    std::vector<Map::Wave>                     _waves;
    std::vector<std::shared_ptr<rtype::Block>> _blockEntities;
    std::vector<std::pair<std::string, float>> _backgrounds;
    bool                                       _endBoss;
    Map::Boss                                  _boss;
    sf::Vector2f                               _endPosition = {-1, -1};
    bool                                       _isLoaded    = false;
    bool                                       _isLevelRunning;
};

#endif /* !ServerMap_HPP_ */
