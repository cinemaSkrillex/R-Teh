/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** PlayerUtils
*/

#include "../include/PlayerUtils.hpp"

namespace PeterParser {
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

// parseDirection and parsePosition are now parseVector2f
sf::Vector2f parseVector2f(const std::string& vector2fStr) {
    sf::Vector2f direction(0.f, 0.f);  // Default to (0, 0) in case of a parsing error

    // Regex to match floating-point numbers inside parentheses
    std::regex  directionRegex(R"(\(([-+]?\d*\.?\d+),([-+]?\d*\.?\d+)\))");
    std::smatch match;

    if (std::regex_search(vector2fStr, match, directionRegex)) {
        // Convert the extracted strings to floats
        direction.x = std::stof(match[1].str());
        direction.y = std::stof(match[2].str());
    } else {
        std::cerr << "Failed to parse direction: " << vector2fStr << std::endl;
    }

    return direction;
}

const sf::Vector2f parseVector2Const(const std::string& Vector2fConst) {
    return parseVector2f(Vector2fConst);
}

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

std::vector<float> parseVelocity(const std::string& velocity) {
    // turn (500,0,{500,500},0)
    // into 500,0,500,500,0
    std::string cleanedVelocity = velocity;
    cleanedVelocity.erase(std::remove(cleanedVelocity.begin(), cleanedVelocity.end(), '('),
                          cleanedVelocity.end());
    cleanedVelocity.erase(std::remove(cleanedVelocity.begin(), cleanedVelocity.end(), ')'),
                          cleanedVelocity.end());
    cleanedVelocity.erase(std::remove(cleanedVelocity.begin(), cleanedVelocity.end(), '{'),
                          cleanedVelocity.end());
    cleanedVelocity.erase(std::remove(cleanedVelocity.begin(), cleanedVelocity.end(), '}'),
                          cleanedVelocity.end());
    std::replace(cleanedVelocity.begin(), cleanedVelocity.end(), ',', ' ');
    std::istringstream stream(cleanedVelocity);
    std::vector<float> values;
    float              value;
    while (stream >> value) {
        values.push_back(value);
    }
    return values;
}

std::vector<long int> parseIds(const std::string& ids) {
    // ex: [1,2,3,4]
    std::string cleanedIds = ids;
    cleanedIds.erase(std::remove(cleanedIds.begin(), cleanedIds.end(), '['), cleanedIds.end());
    cleanedIds.erase(std::remove(cleanedIds.begin(), cleanedIds.end(), ']'), cleanedIds.end());
    std::replace(cleanedIds.begin(), cleanedIds.end(), ',', ' ');
    std::istringstream stream(cleanedIds);
    std::vector<long int> values;
    long int              value;
    while (stream >> value) {
        values.push_back(value);
    }
    return values;
}

}  // namespace PeterParser