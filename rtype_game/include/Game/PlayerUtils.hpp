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
#include <string>
#include <unordered_map>
#include <vector>

struct PlayerData {
    std::string  uuid;
    sf::Vector2f position;
};

PlayerData                                   parsePlayerData(const std::smatch& match);
std::string                                  sanitizeUUID(const std::string& uuid);
const sf::Vector2f                           parsePosition(const std::string& positionStr);
std::vector<PlayerData>                      parsePlayerList(const std::string& playerList);
std::unordered_map<std::string, std::string> parseMessage(const std::string& message);

#endif /* !PLAYERUTILS_HPP_ */
