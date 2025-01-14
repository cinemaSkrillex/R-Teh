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

void RtypeServer::broadcastPlayerState(const Player& player) {
    // Get the player's position
    std::shared_ptr<RealEngine::Entity> entity = player.getEntity();
    if (!entity) {
        return;
    }
    auto& registry = _game_instance->getRegistryRef();
    auto* position = registry.get_component<RealEngine::Position>(*entity);
    if (!position) {
        return;
    }

    // Create a PlayerMoveMessage
    RTypeProtocol::PlayerMoveMessage playerMoveMessage = {};
    playerMoveMessage.message_type                     = RTypeProtocol::PLAYER_MOVE;
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

void RtypeServer::broadcastStartLevel() {
    RTypeProtocol::LevelSignalMessage levelSignalMessage = {};
    levelSignalMessage.message_type                      = RTypeProtocol::LEVEL_SIGNAL;
    levelSignalMessage.startLevel = _game_instance->getMap()->getIsLevelRunning();

    // Serialize the LevelSignalMessage
    std::array<char, 800> serializedMessage = RTypeProtocol::serialize<800>(levelSignalMessage);

    // Broadcast the serialized message to all clients
    broadcastAllReliable(serializedMessage);
}

void RtypeServer::startAndBroadcastLevel() {
    _game_instance->getMap()->startLevel();
    broadcastStartLevel();
}

void RtypeServer::broadcastEntityState(RealEngine::Entity entity, RealEngine::Registry* registry) {
    // broadcast position, angle
    auto* position = registry->get_component<RealEngine::Position>(entity);
    auto* rotation = registry->get_component<RealEngine::Rotation>(entity);

    if (!position) {
        return;
    }

    RTypeProtocol::EntityUpdateMessage entityStateMessage = {};
    entityStateMessage.message_type                       = RTypeProtocol::ENTITY_UPDATE;
    entityStateMessage.uuid                               = entity;
    entityStateMessage.x                                  = position->x;
    entityStateMessage.y                                  = position->y;
    if (rotation) {
        entityStateMessage.angle = rotation->angle;
    } else {
        entityStateMessage.angle = -1;
    }
    entityStateMessage.step                               = _deltaTimeBroadcast;
    entityStateMessage.timestamp = std::chrono::system_clock::now().time_since_epoch().count();

    // Serialize the EntityUpdateMessage
    std::array<char, 800> serializedMessage = RTypeProtocol::serialize<800>(entityStateMessage);

    // Broadcast the serialized message to all clients
    broadcastAllUnreliable(serializedMessage);
}

void RtypeServer::broadcastAllReliable(const std::array<char, 800>& message) {
    for (const auto& client : _server->getClients()) {
        _server->send_reliable_packet(message, client);
    }
}

void RtypeServer::broadcastAllUnreliable(const std::array<char, 800>& message) {
    for (const auto& client : _server->getClients()) {
        _server->send_unreliable_packet(message, client);
    }
}

void RtypeServer::printServerStartupBanner() {
    std::string  timestamp   = formatTimestamp(_startTime);
    int          server_tick = _server_config.getConfigItem<int>("SERVER_TICK");
    sf::Vector2f player_start_position =
        _server_config.getConfigItem<sf::Vector2f>("PLAYER_START_POSITION");
    int  server_broadcast_tick = _server_config.getConfigItem<int>("SERVER_BROADCAST_TICK");
    auto GameMap               = _game_instance->getMap();
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