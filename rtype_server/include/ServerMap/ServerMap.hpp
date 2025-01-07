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

struct Tile {
    std::string             element;        // e.g., "background"
    std::string             type;           // e.g., "block_1"
    std::pair<float, float> position;       // (x, y)
    std::string             collisionType;  // e.g., "SOLID", "NONE"
};

class ServerMap {
   private:
    std::string       _map_name;
    std::vector<Tile> _tiles;  // Updated to store richer tile data

    // JSON helpers
    Json::Value readJSONFile(const std::string& filepath);
    void        writeJSONFile(const std::string& filepath, const Json::Value& json);

   public:
    ServerMap();
    ~ServerMap();

    void loadFromJSON(const std::string& filepath);
    void saveToJSON(const std::string& filepath);

    // Accessor methods
    const std::vector<Tile>& getTiles() const { return _tiles; }
    void                     addTile(const Tile& tile) { _tiles.push_back(tile); }
    // Accessor methods (omitted here for brevity but same as before)
    // Modifiers and debug methods (omitted here for brevity)
};

#endif /* !SERVERMAP_HPP_ */
