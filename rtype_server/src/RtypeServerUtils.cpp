/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RtypeServerUtils
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

sf::Vector2f RtypeServer::parseDirection(const std::string& directionStr) {
    sf::Vector2f direction(0.f, 0.f);  // Default to (0, 0) in case of a parsing error

    // Regex to match floating-point numbers inside parentheses
    std::regex  directionRegex(R"(\(([-+]?\d*\.?\d+),([-+]?\d*\.?\d+)\))");
    std::smatch match;

    if (std::regex_search(directionStr, match, directionRegex)) {
        // Convert the extracted strings to floats
        direction.x = std::stof(match[1].str());
        direction.y = std::stof(match[2].str());
    } else {
        std::cerr << "Failed to parse direction: " << directionStr << std::endl;
    }

    return direction;
}

std::string RtypeServer::formatTimestamp(const std::chrono::steady_clock::time_point& start_time) {
    auto now     = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count();
    return std::to_string(elapsed);  // in milliseconds
}

void RtypeServer::broadcastPlayerState(const Player& player) {
    // Get the player's position
    RealEngine::Entity* entity = player.getEntity();
    auto* position = _game_instance->getRegistryRef().get_component<RealEngine::Position>(*entity);
    if (position) {
        std::string message = "Event:Player_position Uuid:" + std::to_string(player.getUUID()) +
                              " Position:(" + std::to_string(position->x) + "," +
                              std::to_string(position->y) + ")";
        for (auto client : _server->getClients()) {
            _server->send_unreliable_packet(message, client);
        }
    }
}