/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** GameMap
*/

#include "Game/GameMap.hpp"

GameMap::GameMap(RealEngine::Registry& registry) : _registry(registry), _isLevelRunning(false) {}

GameMap::~GameMap() { std::cout << "GameMap destroyed" << std::endl; }

Json::Value GameMap::readJSONFile(const std::string& filepath) {
    try {
        if (!std::filesystem::exists(filepath)) {
            throw std::runtime_error("JSON file does not exist: " + filepath);
        }
        Json::Value root;

        std::ifstream file(filepath, std::ifstream::binary);

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

void GameMap::updateLevel(float deltaTime) {
    // if (!_isLevelRunning) {
    //     return;
    // }
    x_level_position += _scrollingSpeed * deltaTime;
    for (auto& block : _blockEntities) {
        auto* position = _registry.get_component<RealEngine::Position>(block->getEntity());
        if (position) {
            position->x -= _scrollingSpeed * deltaTime;
        }
    }
    removeDeadBlocks();
}

std::vector<Map::WaveMob> GameMap::invokeWaves() {
    // if (!_isLevelRunning) {
    //     return {};
    // }
    std::vector<Map::WaveMob> enemiesToSpawn;
    for (auto& wave : _waves) {
        if (x_level_position < wave.startPosition.x) {
            break;
        } else {
            enemiesToSpawn.insert(enemiesToSpawn.end(), wave.mobs.begin(), wave.mobs.end());
            _waves.erase(_waves.begin());
        }
    }
    return enemiesToSpawn;
}

void GameMap::unloadLevel() {
    _isLevelRunning = false;
    for (auto& block : _blockEntities) {
        if (block) _registry.add_component(block->getEntity(), RealEngine::AutoDestructible{0.0f});
    }
    _blockEntities.clear();
    if (_boss.bossEntity) {
        _registry.add_component(_boss.bossEntity, RealEngine::AutoDestructible{0.0f});
    }
    _scrollingSpeed  = 0.0f;
    x_level_position = 0.0f;
    _isLoaded        = false;
}

void GameMap::startLevel() { _isLevelRunning = true; }

void GameMap::stopLevel() { _isLevelRunning = false; }

void GameMap::writeJSONFile(const std::string& filepath, const Json::Value& json) {
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

void GameMap::loadFromJSON(const std::string& filepath) {
    Json::Value root = readJSONFile(filepath);
    try {
        _map_name = root["map_name"].asString();
        _scrollingSpeed =
            root["scrollingSpeed"]
                .asFloat();  // if scrollingSpeed is not present, it will be 1.0f (thanks clamping)
        _scrollingSpeed = std::clamp(_scrollingSpeed, 10.0f, 1000.0f);
        _music_name     = root["music"].asString();
        // for all backgrounds, parse     std::vector<std::pair<std::string, float>> _backgrounds;
        for (const auto& background : root["backgrounds"]) {
            std::string background_str = background["sprite"].asString();
            float       speed          = background["speed"].asFloat();
            _backgrounds.push_back({background_str, speed});
        }

        // Load tiles
        const auto& tiles = root["mapData"]["tiles"];
        for (const auto& tileJson : tiles) {
            Map::Tile tile;
            tile.type     = tileJson["type"].asString();
            tile.element  = tileJson["element"].asString();
            tile.position = {tileJson["position"][0].asFloat(), tileJson["position"][1].asFloat()};
            tile.rotation = tileJson["rotation"].asFloat();
            _tiles.push_back(tile);
            if (tile.type == "BLOCK") {
                auto block = std::make_shared<rtype::Block>(_registry, tile.position, tile.element,
                                                            tile.rotation);
                _blockEntities.push_back(block);
            }
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
            std::string path = "../../assets/maps/waves/";
            std::string waveFilePath = path + wave.waveType + ".json";
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

        std::sort(_waves.begin(), _waves.end(), [](const Map::Wave& a, const Map::Wave& b) {
            return a.startPosition.x < b.startPosition.x;
        });

    } catch (const std::exception& e) {
        std::cerr << "Error loading waves: " << e.what() << std::endl;
    }
    _isLoaded = true;
}

void GameMap::saveToJSON(const std::string& filepath) {
    Json::Value root;

    // Save map name
    _map_name = root["map_name"].asString();
    // Save tiles
    for (const auto& tile : _tiles) {
        Json::Value tileJson;
        tileJson["element"] = Json::Value(tile.element);
        tileJson["type"]    = Json::Value(tile.type);
        tileJson["position"].append(tile.position.x);  // x-coordinate
        tileJson["position"].append(tile.position.y);  // y-coordinate
        tileJson["rotation"] = tile.rotation;
        root["mapData"]["tiles"].append(tileJson);
    }

    // Save waves
    for (const auto& wave : _waves) {
        Json::Value waveJson;
        waveJson["waveType"] = Json::Value(wave.waveType);
        waveJson["startPosition"].append(wave.startPosition.x);
        waveJson["startPosition"].append(wave.startPosition.y);

        // Save wave contents to a separate file
        Json::Value waveRoot;
        for (const auto& wabeMob : wave.mobs) {
            Json::Value contentJson;
            contentJson["mobName"] = Json::Value(wabeMob.name);
            contentJson["position"].append(wabeMob.position.x);
            contentJson["position"].append(wabeMob.position.y);
            waveRoot["wave"] = contentJson;
        }
        std::string path = "../../assets/maps/waves/";
        std::string waveFilePath = path + wave.waveType + ".json";
        writeJSONFile(waveFilePath, waveRoot);

        root["mapData"]["waves"].append(waveJson);
    }

    writeJSONFile(filepath, root);
}

void GameMap::removeDeadBlocks() {
    _blockEntities.erase(
        std::remove_if(_blockEntities.begin(), _blockEntities.end(),
                       [](const std::shared_ptr<rtype::Block>& block) { return block == nullptr; }),
        _blockEntities.end());
}
