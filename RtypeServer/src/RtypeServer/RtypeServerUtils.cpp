/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RtypeServerUtils
*/

#include "../../include/RtypeServer/RtypeServer.hpp"
#include "TerminalColors.hpp"

std::string RtypeServer::formatTimestamp(const std::chrono::steady_clock::time_point& start_time) {
    auto now     = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count();
    return std::to_string(elapsed);  // in milliseconds
}

void RtypeServer::broadcastPlayerState(const ServerPlayer& player) {
    // Get the player's position
    std::shared_ptr<RealEngine::Entity> entity = player.getEntity();
    if (!entity) {
        return;
    }
    auto& registry        = _gameInstance->getRegistryRef();
    auto* position        = registry.getComponent<RealEngine::Position>(*entity);
    auto* netvarContainer = registry.getComponent<RealEngine::NetvarContainer>(*entity);

    if (netvarContainer) {
        auto* score_health_update = netvarContainer->getNetvar("score_health_update");
        if (score_health_update) {
            if (std::any_cast<bool>(score_health_update->value)) {
                std::cout << "Score health update : "
                          << std::any_cast<bool>(score_health_update->value) << std::endl;
                auto* score  = registry.getComponent<RealEngine::Score>(*entity);
                auto* health = registry.getComponent<RealEngine::Health>(*entity);

                RTypeProtocol::PlayerUpdateDataMessage playerUpdateDataMessage = {};
                playerUpdateDataMessage.messageType = RTypeProtocol::PLAYER_UPDATE_DATA;
                playerUpdateDataMessage.uuid        = player.getUUID();
                if (score) {
                    std::cout << "Score amount : " << score->amount << std::endl;
                    std::cout << "PlayerUpdateDataMessage score : " << playerUpdateDataMessage.score
                              << std::endl;
                    if (score->amount > playerUpdateDataMessage.score) {
                        playerUpdateDataMessage.score = score->amount;
                        _log->log("Player " + std::to_string(player.getUUID()) +
                                  " has a new best score: " + std::to_string(score->amount));
                    } else {
                        _log->log("Player " + std::to_string(player.getUUID()) + " score " +
                                  std::to_string(score->amount) +
                                  " is not higher than their current best score: " +
                                  std::to_string(playerUpdateDataMessage.score));
                    }
                } else {
                    playerUpdateDataMessage.score = -1;
                }
                if (health) {
                    playerUpdateDataMessage.health = health->amount;
                } else {
                    playerUpdateDataMessage.health = -1;
                }

                // Serialize the PlayerUpdateDataMessage
                std::array<char, 800> serializedMessage =
                    RTypeProtocol::serialize<800>(playerUpdateDataMessage);

                // Broadcast the serialized message to all clients
                broadcastAllReliable(serializedMessage);
                score_health_update->value = false;
            }
        }
    }

    if (!position) {
        return;
    }

    // Create a PlayerMoveMessage
    RTypeProtocol::PlayerMoveMessage playerMoveMessage = {};
    playerMoveMessage.messageType                      = RTypeProtocol::PLAYER_MOVE;
    playerMoveMessage.uuid                             = player.getUUID();
    playerMoveMessage.x                                = position->x;
    playerMoveMessage.y                                = position->y;
    playerMoveMessage.step                             = _deltaTimeBroadcast;
    playerMoveMessage.timestamp = std::chrono::system_clock::now().time_since_epoch().count();

    // Serialize the PlayerMoveMessage
    std::array<char, 800> serializedMessage = RTypeProtocol::serialize<800>(playerMoveMessage);

    // Broadcast the serialized message to all clients
    broadcastAllUnreliable(serializedMessage);
}

void RtypeServer::startAndBroadcastLevel() {
    _gameInstance->getMap()->startLevel();
    broadcastStartLevel();
}

void RtypeServer::broadcastEntityState(RealEngine::Entity entity, RealEngine::Registry* registry) {
    // broadcast position, angle
    auto* position = registry->getComponent<RealEngine::Position>(entity);
    auto* rotation = registry->getComponent<RealEngine::Rotation>(entity);

    if (!position) {
        return;
    }

    RTypeProtocol::EntityUpdateMessage entityStateMessage = {};
    entityStateMessage.messageType                        = RTypeProtocol::ENTITY_UPDATE;
    entityStateMessage.uuid                               = entity;
    entityStateMessage.x                                  = position->x;
    entityStateMessage.y                                  = position->y;
    if (rotation) {
        entityStateMessage.angle = rotation->angle;
    } else {
        entityStateMessage.angle = -1;
    }
    entityStateMessage.step      = _deltaTimeBroadcast;
    entityStateMessage.timestamp = std::chrono::system_clock::now().time_since_epoch().count();

    // Serialize the EntityUpdateMessage
    std::array<char, 800> serializedMessage = RTypeProtocol::serialize<800>(entityStateMessage);

    // Broadcast the serialized message to all clients
    broadcastAllUnreliable(serializedMessage);
}

void RtypeServer::broadcastAllReliable(const std::array<char, 800>& message) {
    for (const auto& client : _server->getClients()) {
        _server->sendReliablePacket(message, client);
    }
}

void RtypeServer::broadcastAllUnreliable(const std::array<char, 800>& message) {
    for (const auto& client : _server->getClients()) {
        _server->sendUnreliablePacket(message, client);
    }
}

void RtypeServer::printServerStartupBanner() {
    std::string  timestamp   = formatTimestamp(_startTime);
    int          server_tick = _serverConfig.getConfigItem<int>("SERVER_TICK");
    sf::Vector2f player_start_position =
        _serverConfig.getConfigItem<sf::Vector2f>("PLAYER_START_POSITION");
    int  server_broadcast_tick = _serverConfig.getConfigItem<int>("SERVER_BROADCAST_TICK");
    auto GameMap               = _gameInstance->getMap();
    auto blockEntities         = GameMap->getBlockEntities();
    auto waveEntities          = GameMap->getWaves();
    auto tiles                 = GameMap->getTiles();
    auto scrollSpeed           = GameMap->getScrollingSpeed();
    auto XLevelPosition        = GameMap->getXLevelPosition();
    auto isLoaded              = GameMap->isLoaded();
    auto IP                    = _server->getLocalEndpoint().address().to_string();
    auto PORT                  = _server->getLocalEndpoint().port();

    std::cout << colorText("=========================================", BOLD_GREEN) << std::endl;
    std::cout << boldColorText("R-Type Server", BOLD_GREEN) << " - "
              << colorText("Server Started", BOLD_YELLOW) << std::endl;
    std::cout << colorText("=========================================", BOLD_GREEN) << std::endl;
    std::cout << colorText("[Server] IP: ", BOLD_CYAN) << colorText(IP, BOLD_WHITE) << std::endl;
    std::cout << colorText("[Server] Port: ", BOLD_CYAN)
              << colorText(std::to_string(PORT), BOLD_WHITE) << std::endl;
    std::cout << colorText("[ServerConfig] ServerTickRate: ", BOLD_CYAN)
              << colorText(std::to_string(server_tick), BOLD_WHITE) << std::endl;
    std::cout << colorText("[ServerConfig] ServerBroadcastTickRate: ", BOLD_CYAN)
              << colorText(std::to_string(server_broadcast_tick), BOLD_WHITE) << std::endl;
    std::cout << colorText("[ServerConfig] PlayerStartPosition: ", BOLD_CYAN)
              << colorText("(" + std::to_string(player_start_position.x) + "," +
                               std::to_string(player_start_position.y) + ")",
                           BOLD_WHITE)
              << std::endl;
    std::cout << colorText("[GameMap] ScrollingSpeed: ", BOLD_CYAN)
              << colorText(std::to_string(scrollSpeed), BOLD_WHITE) << std::endl;
    std::cout << colorText("[GameMap] XLevelPosition: ", BOLD_CYAN)
              << colorText(std::to_string(XLevelPosition), BOLD_WHITE) << std::endl;
    std::cout << colorText("[GameMap] BlockEntities: ", BOLD_CYAN)
              << colorText(std::to_string(blockEntities.size()), BOLD_WHITE) << std::endl;
    std::cout << colorText("[GameMap] Tiles: ", BOLD_CYAN)
              << colorText(std::to_string(tiles.size()), BOLD_WHITE) << std::endl;
    std::cout << colorText("[GameMap] WaveEntities: ", BOLD_CYAN)
              << colorText(std::to_string(waveEntities.size()), BOLD_WHITE) << std::endl;
    std::cout << colorText("[GameMap] IsLoaded: ", BOLD_CYAN)
              << colorText(isLoaded ? "true" : "false", BOLD_WHITE) << std::endl;
    std::cout << colorText("=========================================", BOLD_GREEN) << std::endl;
}

void RtypeServer::broadcastStartLevelUtils() {
    RTypeProtocol::LevelSignalMessage levelSignalMessage = {};
    levelSignalMessage.messageType                       = RTypeProtocol::LEVEL_SIGNAL;
    levelSignalMessage.startLevel = _gameInstance->getMap()->getIsLevelRunning();
    std::cout << "getMap()->getIsLevelRunning() = " << _gameInstance->getMap()->getIsLevelRunning()
              << std::endl;
    std::cout << "levelSignalMessage.startLevel = " << levelSignalMessage.startLevel << std::endl;
    // Serialize the LevelSignalMessage
    std::array<char, 800> serializedMessage = RTypeProtocol::serialize<800>(levelSignalMessage);

    // Broadcast the serialized message to all clients
    broadcastAllReliable(serializedMessage);
}

void RtypeServer::broadcastStartLevel() {
    broadcastStartLevel();
    std::cout << "broadcasting start level" << std::endl;
}

void RtypeServer::broadcastStopLevel() {
    broadcastStartLevel();
    std::cout << "broadcasting stop level" << std::endl;
    // we get the variables from getIsLevelRunning() and we broadcast it to all clients
}

void RtypeServer::startLevel() {
    _gameInstance->getMap()->startLevel();
    std::cout << "Level started" << std::endl;
}

void RtypeServer::stopLevel() {
    _gameInstance->getMap()->stopLevel();
    std::cout << "Level stopped" << std::endl;
}