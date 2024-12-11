/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** PlayerUtils
*/

#include "PlayerUtils.hpp"

// this is in the library
std::unordered_map<std::string, std::string> parseMessage(const std::string& message) {
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

// this is now parseVector2f
const sf::Vector2f parsePosition(const std::string& positionStr) {
    sf::Vector2f position(0, 0);  // Default to (0, 0) in case of a parsing error

    // Updated regex to handle floating-point numbers
    std::regex  positionRegex(R"(\(([-+]?\d*\.?\d+),([-+]?\d*\.?\d+)\))");
    std::smatch match;

    if (std::regex_search(positionStr, match, positionRegex)) {
        // Convert the extracted strings to floats
        position.x = std::stof(match[1].str());
        position.y = std::stof(match[2].str());
    } else {
        std::cerr << "Failed to parse position: " << positionStr << std::endl;
    }

    return position;
}

// Function to parse the player list string (without the surrounding brackets)
std::vector<PlayerData> parsePlayerList(const std::string& playerList) {
    std::vector<PlayerData> players;

    // Remove the surrounding brackets
    if (playerList.size() >= 2 && playerList.front() == '[' && playerList.back() == ']') {
        std::string cleanedList =
            playerList.substr(1, playerList.length() - 2);  // Removes "[" and "]"

        // Regex to match player data
        std::regex  playerRegex(R"(\|?([^,]+),\(([-+]?\d*\.?\d+),([-+]?\d*\.?\d+)\))");
        std::smatch match;

        std::string::const_iterator searchStart(cleanedList.cbegin());
        while (std::regex_search(searchStart, cleanedList.cend(), match, playerRegex)) {
            if (match.size() == 4) {
                PlayerData player = parsePlayerData(match);
                // sanitizing the UUID is done inside "parsePlayerData"
                players.push_back(player);
                std::cout << "Parsed player - UUID: " << player.uuid << ", Position: ("
                          << player.position.x << ", " << player.position.y << ")" << std::endl;
            } else {
                std::cerr << "Malformed player data: " << match.str() << std::endl;
            }

            // Move the search start to the end of the current match
            searchStart = match.suffix().first;
        }
    } else {
        std::cerr << "Malformed player list: " << playerList << std::endl;
    }

    return players;
}

PlayerData parsePlayerData(const std::smatch& match) {
    PlayerData player;
    player.uuid     = match[1].str();
    player.position = sf::Vector2f(std::stof(match[2].str()), std::stof(match[3].str()));

    // Sanitize UUID
    player.uuid = sanitizeUUID(player.uuid);

    return player;
}

std::string sanitizeUUID(const std::string& uuid) {
    std::string sanitizedUUID = uuid;
    sanitizedUUID.erase(std::remove(sanitizedUUID.begin(), sanitizedUUID.end(), '|'),
                        sanitizedUUID.end());
    return sanitizedUUID;
}