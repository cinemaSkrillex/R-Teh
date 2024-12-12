/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** ServerConfig
*/

#ifndef SERVERCONFIG_HPP_
#define SERVERCONFIG_HPP_

#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

class ServerConfig {
   public:
    sf::Int32    SERVER_TICK;
    sf::Int32    SERVER_BROADCAST_TICK;
    sf::Vector2f PLAYER_START_POSITION;
    float        SNAP_THRESHOLD;

    ServerConfig(const std::string& configFilePath) {
        std::ifstream configFile(configFilePath);
        if (!configFile.is_open()) {
            throw std::runtime_error("Could not open config file");
        }

        std::unordered_map<std::string, std::string> configMap;
        std::string                                  line;
        while (std::getline(configFile, line)) {
            std::istringstream lineStream(line);
            std::string        key, value;
            if (std::getline(lineStream, key, '=') && std::getline(lineStream, value)) {
                configMap[key] = value;
            }
        }

        SERVER_TICK           = std::stoi(configMap["SERVER_TICK"]);
        SERVER_BROADCAST_TICK = std::stoi(configMap["SERVER_BROADCAST_TICK"]);
        PLAYER_START_POSITION = sf::Vector2f(std::stof(configMap["PLAYER_START_POSITION_X"]),
                                             std::stof(configMap["PLAYER_START_POSITION_Y"]));
        SNAP_THRESHOLD        = std::stof(configMap["SNAP_THRESHOLD"]);

        configFile.close();

        std::cout << "SERVER_TICK: " << SERVER_TICK << std::endl;
        std::cout << "SERVER_BROADCAST_TICK: " << SERVER_BROADCAST_TICK << std::endl;
        std::cout << "PLAYER_START_POSITION: (" << PLAYER_START_POSITION.x << ", "
                  << PLAYER_START_POSITION.y << ")" << std::endl;
        std::cout << "SNAP_THRESHOLD: " << SNAP_THRESHOLD << std::endl;
    }
};

#endif /* !SERVERCONFIG_HPP_ */
