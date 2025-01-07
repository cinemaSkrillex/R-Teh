/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** ServerMap
*/

#include "ServerMap/ServerMap.hpp"

ServerMap::ServerMap() {}

ServerMap::~ServerMap() { std::cout << "ServerMap destroyed" << std::endl; }

Json::Value ServerMap::readJSONFile(const std::string& filepath) {
    if (!std::filesystem::exists(filepath)) {
        throw std::runtime_error("JSON file does not exist: " + filepath);
    }
    Json::Value root;

    std::ifstream file(filepath, std::ifstream::binary);
    std::cout << "Reading JSON file: " << filepath << std::endl;

    if (!file.is_open()) {
        throw std::runtime_error("Unable to open JSON file: " + filepath);
    }

    try {
        file >> root;
    } catch (const Json::Exception& e) {
        throw std::runtime_error("Error parsing JSON file: " + std::string(e.what()));
    }

    file.close();
    return root;
}

void ServerMap::writeJSONFile(const std::string& filepath, const Json::Value& json) {
    std::ofstream file(filepath, std::ofstream::binary);

    if (!file.is_open()) {
        throw std::runtime_error("Unable to open JSON file for writing: " + filepath);
    }

    try {
        file << json.toStyledString();
    } catch (const Json::Exception& e) {
        throw std::runtime_error("Error writing JSON to file: " + std::string(e.what()));
    }

    file.close();
}

void ServerMap::loadFromJSON(const std::string& filepath) {
    Json::Value root = readJSONFile(filepath);
    std::cout << "Loaded map from JSON file: " << filepath << std::endl;
    // Load map name
    _map_name = root["map_name"].asString();

    // Load tiles
    const auto& tiles = root["mapData"]["tiles"];
    for (const auto& tileJson : tiles) {
        RTypeProtocol::Tile tile;
        tile.element       = tileJson["element"].asString();
        tile.type          = tileJson["type"].asString();
        tile.position      = {tileJson["position"][0].asFloat(), tileJson["position"][1].asFloat()};
        tile.collisionType = tileJson["collisionType"].asString();

        std::cout << "Loaded tile: " << tile.element << " " << tile.type << " "
                  << tile.position.first << " " << tile.position.second << " " << tile.collisionType
                  << std::endl;
        _tiles.push_back(tile);
    }
}

void ServerMap::saveToJSON(const std::string& filepath) {
    Json::Value root;

    // Save map name
    root["map_name"] = _map_name;

    // Save tiles
    for (const auto& tile : _tiles) {
        Json::Value tileJson;
        tileJson["element"] = tile.element;
        tileJson["type"]    = tile.type;
        tileJson["position"].append(tile.position.first);   // x-coordinate
        tileJson["position"].append(tile.position.second);  // y-coordinate
        tileJson["collisionType"] = tile.collisionType;
        root["mapData"]["tiles"].append(tileJson);
    }

    writeJSONFile(filepath, root);
}
