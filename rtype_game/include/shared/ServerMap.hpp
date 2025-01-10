/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** ServerMap
*/

#ifndef SERVERMAP_HPP_
#define SERVERMAP_HPP_

#include <json/json.h>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "Engine.hpp"
#include "RtypeServerProtocol.hpp"

namespace Map {
struct Tile {
    std::string  type;      // e.g., "BLOCK", "NONE"
    std::string  element;   // e.g., "background"
    sf::Vector2f position;  // e.g., (0, 0)
    float        rotation;  // e.g., (90.0)
};

struct WaveContent {
    std::string  mobName;   // Name of the mob (e.g., "eye_minion")
    sf::Vector2f position;  // Position of the mob
};

struct Wave {
    std::string              waveType;       // e.g., "wave1" (filename of the wave JSON)
    sf::Vector2f             startPosition;  // Starting position for the wave
    std::vector<WaveContent> contents;       // List of mobs in this wave
};
}  // namespace Map

class ServerMap {
   private:
    std::string            _map_name;
    std::vector<Map::Tile> _tiles;
    std::vector<Map::Wave> _waves;

    // JSON helpers
    Json::Value readJSONFile(const std::string& filepath);
    void        writeJSONFile(const std::string& filepath, const Json::Value& json);

   public:
    ServerMap();
    ~ServerMap();

    void loadFromJSON(const std::string& filepath);
    void saveToJSON(const std::string& filepath);

    // Accessor methods
    const std::vector<Map::Tile>& getTiles() const { return _tiles; }
    const std::vector<Map::Wave>& getWaves() const { return _waves; }
    void                          addTile(const Map::Tile& tile) { _tiles.push_back(tile); }
    // Accessor methods (omitted here for brevity but same as before)
    // Modifiers and debug methods (omitted here for brevity)
};

#endif /* !SERVERMAP_HPP_ */
