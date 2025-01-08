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

class ServerMap {
   private:
    std::string                      _map_name;
    std::vector<RTypeProtocol::Tile> _tiles;  // Updated to store richer tile data

    // JSON helpers
    Json::Value readJSONFile(const std::string& filepath);
    void        writeJSONFile(const std::string& filepath, const Json::Value& json);

   public:
    ServerMap();
    ~ServerMap();

    void loadFromJSON(const std::string& filepath);
    void saveToJSON(const std::string& filepath);

    // Accessor methods
    const std::vector<RTypeProtocol::Tile>& getTiles() const { return _tiles; }
    void addTile(const RTypeProtocol::Tile& tile) { _tiles.push_back(tile); }
    // Accessor methods (omitted here for brevity but same as before)
    // Modifiers and debug methods (omitted here for brevity)
};

#endif /* !SERVERMAP_HPP_ */
