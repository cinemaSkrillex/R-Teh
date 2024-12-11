/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** PlayerUtils
*/

// #include "PlayerUtils.hpp"

namespace PeterParser {
// std::unordered_map<std::string, std::string> parseMessage(const std::string& message) {
//     std::unordered_map<std::string, std::string> parsed_data;

//     // Break the message into key-value pairs using a delimiter
//     std::istringstream stream(message);
//     std::string        token;

//     // Process each key-value pair
//     while (std::getline(stream, token, ' ')) {  // ' ' as delimiter for key:value pairs
//         auto delimiter_pos = token.find(':');
//         if (delimiter_pos != std::string::npos) {
//             std::string key   = token.substr(0, delimiter_pos);
//             std::string value = token.substr(delimiter_pos + 1);
//             parsed_data[key]  = value;  // Store in the map
//         }
//     }

//     return parsed_data;
// }

// // parseDirection and parsePosition are now parseVector2f
// sf::Vector2f parseVector2f(const std::string& directionStr) {
//     sf::Vector2f direction(0.f, 0.f);  // Default to (0, 0) in case of a parsing error

//     // Regex to match floating-point numbers inside parentheses
//     std::regex  directionRegex(R"(\(([-+]?\d*\.?\d+),([-+]?\d*\.?\d+)\))");
//     std::smatch match;

//     if (std::regex_search(directionStr, match, directionRegex)) {
//         // Convert the extracted strings to floats
//         direction.x = std::stof(match[1].str());
//         direction.y = std::stof(match[2].str());
//     } else {
//         std::cerr << "Failed to parse direction: " << directionStr << std::endl;
//     }

//     return direction;
// }

}  // namespace PeterParser