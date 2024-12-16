/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** ServerConfig
*/

#ifndef SERVERCONFIG_HPP_
#define SERVERCONFIG_HPP_

#include <configpath.h>

#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "configpath.h"

class ServerConfig {
   public:
    ServerConfig() : _configFilePath(CONFIG_FILE_PATH) { loadConfig(); }

    // Template function to get a config item by key and convert it to the desired type
    template <typename T>
    T getConfigItem(const std::string& key) const {
        auto it = _configMap.find(key);
        if (it == _configMap.end()) {
            throw std::runtime_error("Key not found in config file: " + key);
        }
        return convert<T>(it->second);
    }

   private:
    std::string                                  _configFilePath;
    std::unordered_map<std::string, std::string> _configMap;

    template <typename T>
    T convert(const std::string& value) const;

    std::string trim(const std::string& str) const {
        const char* whitespace = " \t\n\r";
        size_t      start      = str.find_first_not_of(whitespace);
        if (start == std::string::npos) return "";  // All whitespace
        size_t end = str.find_last_not_of(whitespace);
        return str.substr(start, end - start + 1);
    }

    void loadConfig() {
        std::ifstream configFile(_configFilePath);
        if (!configFile.is_open()) {
            throw std::runtime_error("Could not open config file: " + _configFilePath);
        }

        std::string line;
        while (std::getline(configFile, line)) {
            if (line.empty() || line[0] == '#') continue;

            std::istringstream lineStream(line);
            std::string        key, value;
            if (std::getline(lineStream, key, '=') && std::getline(lineStream, value)) {
                key   = trim(key);
                value = trim(value);
                _configMap[key] = value;
            }
        }

        configFile.close();
    }
};

// Specializations for supported types
template <>
inline int ServerConfig::convert<int>(const std::string& value) const {
    return std::stoi(value);
}

template <>
inline float ServerConfig::convert<float>(const std::string& value) const {
    return std::stof(value);
}

template <>
inline sf::Vector2f ServerConfig::convert<sf::Vector2f>(const std::string& value) const {
    std::istringstream iss(value);
    std::string        xStr, yStr;
    if (std::getline(iss, xStr, ',') && std::getline(iss, yStr)) {
        return sf::Vector2f(std::stof(xStr), std::stof(yStr));
    } else {
        throw std::runtime_error("Invalid format for sf::Vector2f: " + value);
    }
}

#endif  // SERVERCONFIG_HPP_