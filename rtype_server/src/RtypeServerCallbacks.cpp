/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RtypeServerCallbacks
*/

#include "../include/RtypeServer.hpp"

std::unordered_map<std::string, std::string> RtypeServer::parseMessage(const std::string& message) {
    std::unordered_map<std::string, std::string> parsed_data;

    // Break the message into key-value pairs using a delimiter
    std::istringstream stream(message);
    std::string        token;

    // Process each key-value pair
    while (std::getline(stream, token, ' ')) {  // ' ' as delimiter for key:value pairs
        auto delimiter_pos = token.find(':');
        if (delimiter_pos != std::string::npos) {
            std::string key   = token.substr(0, delimiter_pos);
            std::string value = token.substr(delimiter_pos + 1);
            parsed_data[key]  = value;  // Store in the map
        }
    }

    return parsed_data;
}

std::string RtypeServer::formatTimestamp(const std::chrono::steady_clock::time_point& start_time) {
    auto now     = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count();
    return std::to_string(elapsed);  // in milliseconds
}

void RtypeServer::initCallbacks() {
    _server->setNewClientCallback([this](const asio::ip::udp::endpoint& sender) {
        std::cout << "Callback: New client connected from " << sender.address() << std::endl;

        UUIDGenerator uuid_generator;
        long int      uuid = uuid_generator.generate_long();
        std::cout << "Generated UUID: " << uuid << std::endl;
        // Notify all other clients about the new client
        for (const auto& client : _server->getClients()) {
            if (client != sender) {
                const std::string message = "Event:New_client Uuid:" + std::to_string(uuid) +
                                            " Position:(" +
                                            std::to_string(PLAYER_START_POSITION.x) + "," +
                                            std::to_string(PLAYER_START_POSITION.y) + ")";
                _server->send_reliable_packet(message, client);
            }
        }
        // Create the uuid for each new client
        std::string message = "Event:Synchronize Uuid:" + std::to_string(uuid) +
                              " Clock:" + formatTimestamp(_startTime) + " Position:(" +
                              std::to_string(PLAYER_START_POSITION.x) + "," +
                              std::to_string(PLAYER_START_POSITION.y) + ") Players:[";
        bool first = true;
        for (const auto& player_pair : _players) {
            if (!first) message += "|";
            first = false;
            const auto& player = player_pair.second;
            message += std::to_string(player.getUUID()) + ",(" +
                       std::to_string(player.getPosition().x) + "," +
                       std::to_string(player.getPosition().y) + ")";
        }
        message += "]";
        _server->send_reliable_packet(message, sender);
        long elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(
                                std::chrono::steady_clock::now() - _startTime)
                                .count();
        auto player =
            Player(uuid, elapsed_time, _game_instance->addPlayer(uuid, PLAYER_START_POSITION),
                   _game_instance->getRegistry());
        _players[sender] = player;
    });
}