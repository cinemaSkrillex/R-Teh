/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** PlayerUtils
*/

#include "PlayerUtils.hpp"

std::unordered_map<std::string, std::string> parseMessage(const std::string& message) {
    std::unordered_map<std::string, std::string> parsedData;
    std::istringstream                           stream(message);
    std::string                                  key, value;

    while (stream) {
        if (!std::getline(stream, key, ':')) break;
        if (!std::getline(stream, value, ' ')) break;
        key.erase(0, key.find_first_not_of(" \t"));
        key.erase(key.find_last_not_of(" \t") + 1);
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);

        // Ensure both key and value are properly copied into the map
        parsedData[key] = value;
    }

    return parsedData;
}

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