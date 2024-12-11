/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** PlayerUtils
*/

#ifndef PLAYERUTILS_HPP_
#define PLAYERUTILS_HPP_

#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace PeterParser {

struct PlayerData {
    std::string  uuid;
    sf::Vector2f position;
};

PlayerData                                   parsePlayerData(const std::smatch& match);
std::string                                  sanitizeUUID(const std::string& uuid);
sf::Vector2f                                 parseVector2f(const std::string& vector2fStr);
const sf::Vector2f                           parseVector2Const(const std::string& Vector2fConst);
std::vector<PlayerData>                      parsePlayerList(const std::string& playerList);
std::unordered_map<std::string, std::string> parseMessage(const std::string& message);

}  // namespace PeterParser

#endif /* !PLAYERUTILS_HPP_ */
