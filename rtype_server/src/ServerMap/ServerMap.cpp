/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** ServerMap
*/

#include "shared/ServerMap.hpp"

ServerMap::ServerMap() {}

ServerMap::~ServerMap() { std::cout << "ServerMap destroyed" << std::endl; }

Json::Value ServerMap::readJSONFile(const std::string& filepath) {
    try {
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
    } catch (const std::exception& e) {
        std::cerr << "Error reading JSON file: " << e.what() << std::endl;
        return Json::Value();
    }
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
    try {
        std::cout << "Loaded map from JSON file: " << filepath << std::endl;
        // Load map name
        _map_name = root["map_name"].asString();

        // Load tiles
        const auto& tiles = root["mapData"]["tiles"];
        for (const auto& tileJson : tiles) {
            Map::Tile tile;
            tile.type     = tileJson["type"].asString();
            tile.element  = tileJson["element"].asString();
            tile.position = {tileJson["position"][0].asFloat(), tileJson["position"][1].asFloat()};
            tile.rotation = tileJson["rotation"].asFloat();
            _tiles.push_back(tile);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error loading map: " << e.what() << std::endl;
    }

    // Load waves
    try {
        const auto& waves = root["mapData"]["waves"];
        for (const auto& waveJson : waves) {
            Map::Wave wave;
            wave.waveType      = waveJson["waveType"].asString();
            wave.startPosition = {waveJson["startPosition"][0].asFloat(),
                                  waveJson["startPosition"][1].asFloat()};

            // Load wave contents (from separate JSON file)
            std::string waveFilePath = "../../assets/maps/waves/" + wave.waveType + ".json";
            Json::Value waveRoot     = readJSONFile(waveFilePath);

            for (const auto& waveMobJson : waveRoot["wave"]) {
                Map::WaveMob mobsInWave;
                mobsInWave.name     = waveMobJson["mobName"].asString();
                mobsInWave.position = {waveMobJson["position"][0].asFloat(),
                                       waveMobJson["position"][1].asFloat()};
                wave.mobs.push_back(mobsInWave);
            }
            _waves.push_back(wave);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error loading waves: " << e.what() << std::endl;
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
        tileJson["position"].append(tile.position.x);  // x-coordinate
        tileJson["position"].append(tile.position.y);  // y-coordinate
        tileJson["rotation"] = tile.rotation;
        root["mapData"]["tiles"].append(tileJson);
    }

    // Save waves
    for (const auto& wave : _waves) {
        Json::Value waveJson;
        waveJson["waveType"] = wave.waveType;
        waveJson["startPosition"].append(wave.startPosition.x);
        waveJson["startPosition"].append(wave.startPosition.y);

        // Save wave contents to a separate file
        Json::Value waveRoot;
        for (const auto& wabeMob : wave.mobs) {
            Json::Value contentJson;
            contentJson["mobName"] = wabeMob.name;
            contentJson["position"].append(wabeMob.position.x);
            contentJson["position"].append(wabeMob.position.y);
            waveRoot["wave"] = contentJson;
        }
        std::string waveFilePath = "../../assets/maps/waves/" + wave.waveType + ".json";
        writeJSONFile(waveFilePath, waveRoot);

        root["mapData"]["waves"].append(waveJson);
    }

    writeJSONFile(filepath, root);
}
