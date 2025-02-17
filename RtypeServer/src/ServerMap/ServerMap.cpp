/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** ServerMap
*/

#include "Game/ServerMap.hpp"

ServerMap::ServerMap(RealEngine::Registry& registry)
    : _registry(registry), _isLevelRunning(false) {}

ServerMap::~ServerMap() { std::cout << "ServerMap destroyed" << std::endl; }

Json::Value ServerMap::readJSONFile(const std::string& filepath) {
    try {
        if (!std::filesystem::exists(filepath)) {
            throw std::runtime_error("readJSONFile: JSON file does not exist: " + filepath);
        }
        Json::Value root;

        std::ifstream file(filepath, std::ifstream::binary);

        if (!file.is_open()) {
            throw std::runtime_error("readJSONFile: Unable to open JSON file: " + filepath);
        }

        try {
            file >> root;
        } catch (const Json::Exception& e) {
            throw std::runtime_error("readJSONFile: Error parsing JSON file: " +
                                     std::string(e.what()));
        }

        file.close();
        return root;
    } catch (const std::exception& e) {
        std::cerr << "readJSONFile: Error reading JSON file: " << e.what() << std::endl;
        return Json::Value();
    }
}

void ServerMap::updateLevel(float deltaTime) {
    if (!_isLevelRunning) {
        return;
    }
    _xLevelPosition += _scrollingSpeed * deltaTime;
    if (_blockEntities.empty()) {
        return;
    }

    for (auto it = _blockEntities.begin(); it != _blockEntities.end();) {
        auto& [id, block] = *it;
        if (!_registry.isValid(*block->getEntity())) {
            it = _blockEntities.erase(it);
            continue;
        }
        auto* position = _registry.getComponent<RealEngine::Position>(*block->getEntity());
        if (position) {
            position->x -= _scrollingSpeed * deltaTime;
            if (position->x < -100) {
                _registry.addComponent(block->getEntity(), RealEngine::AutoDestructible{0.0f});
                it = _blockEntities.erase(it);
            } else {
                ++it;
            }
        } else {
            ++it;
        }
    }
    if (!bossAtEnd() && _xLevelPosition >= _endPosition.x) {
        stopLevel();
    } else if (bossAtEnd() && _boss.triggered && !_registry.isValid(*(_boss.bossEntity))) {
        stopLevel();
    }
}

std::vector<Map::WaveMob> ServerMap::invokeLevelMobs() {
    // if (!_isLevelRunning) {
    //     return {};
    // }
    std::vector<Map::WaveMob> enemiesToSpawn;
    for (auto& wave : _waves) {
        if (_xLevelPosition < wave.startPosition.x) {
            break;
        } else {
            enemiesToSpawn.insert(enemiesToSpawn.end(), wave.mobs.begin(), wave.mobs.end());
            _waves.erase(_waves.begin());
        }
    }
    if (bossAtEnd() && _xLevelPosition >= _endPosition.x && !_boss.triggered) {
        enemiesToSpawn.push_back(Map::WaveMob{("boss_" + _boss.bossType), _boss.position});
        _boss.triggered = true;
    }
    return enemiesToSpawn;
}

void ServerMap::unloadLevel() {
    _isLevelRunning = false;
    for (auto& [id, block] : _blockEntities) {
        if (block) {
            std::cout << "removing block " << id << std::endl;
            _registry.addComponent(block->getEntity(), RealEngine::AutoDestructible{0.0f});
        }
    }
    _blockEntities.clear();
    if (_boss.bossEntity) {
        _registry.addComponent(_boss.bossEntity, RealEngine::AutoDestructible{0.0f});
    }
    _boss.bossEntity = nullptr;
    _scrollingSpeed  = 0.0f;
    _xLevelPosition  = 0.0f;
    _isLoaded        = false;
}

void ServerMap::startLevel() { _isLevelRunning = true; }

void ServerMap::stopLevel() { _isLevelRunning = false; }

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
        _mapName = root["map_name"].asString();
        _scrollingSpeed =
            root["scrollingSpeed"]
                .asFloat();  // if scrollingSpeed is not present, it will be 1.0f (thanks clamping)
        _scrollingSpeed = std::clamp(_scrollingSpeed, 10.0f, 1000.0f);
        _musicName      = root["music"].asString();
        for (const auto& background : root["backgrounds"]) {
            std::string background_str = background["sprite"].asString();
            float       speed          = background["speed"].asFloat();
            if (std::find_if(_backgrounds.begin(), _backgrounds.end(),
                             [&background_str](const std::pair<std::string, float>& bg) {
                                 return bg.first == background_str;
                             }) != _backgrounds.end()) {
                continue;
            }
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
                                                            tile.rotation, _scrollingSpeed,
                                                            RealEngine::CollisionType::SOLID);
                _blockEntities[*block->getEntity()] = block;
            }
            if (tile.type == "WAITING_BLOCK") {
                auto waitingBlock = std::make_shared<rtype::WaitingBlock>(
                    _registry, tile.position, tile.element, tile.rotation,
                    RealEngine::CollisionType::SOLID);
                _blockEntities[*waitingBlock->getEntity()] = waitingBlock;
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
            std::string path         = "../../assets/maps/waves/";
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

    // Load end level;

    try {
        const auto& end_level = root["mapData"]["endCondition"];
        const auto& end_type  = end_level["type"].asString();
        if (end_type == "BOSS") {
            _endPosition    = {end_level["triggerPosition"][0].asFloat(),
                               end_level["triggerPosition"][1].asFloat()};
            _boss.position  = {end_level["spawnPosition"][0].asFloat(),
                               end_level["spawnPosition"][1].asFloat()};
            _boss.bossType  = end_level["Entity"].asString();
            _boss.triggered = false;
        } else if (end_type == "POSITION") {
            _endPosition = {end_level["position"][0].asFloat(), end_level["position"][1].asFloat()};
        }
    } catch (const std::exception& e) {
        std::cerr << "Error loading end level: " << e.what() << std::endl;
    }
}

void ServerMap::saveToJSON(const std::string& filepath) {
    Json::Value root;

    // Save map name
    _mapName = root["map_name"].asString();
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
        std::string path         = "../../assets/maps/waves/";
        std::string waveFilePath = path + wave.waveType + ".json";
        writeJSONFile(waveFilePath, waveRoot);

        root["mapData"]["waves"].append(waveJson);
    }

    writeJSONFile(filepath, root);
}