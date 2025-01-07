/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** RtypeEventUtils
*/

#include "../../include/RtypeServer/RtypeServerUtils.hpp"

void addComponentToMessage(RTypeProtocol::NewEntityMessage& message,
                  RTypeProtocol::ComponentList componentType, const std::string& componentData) {
    std::vector<char> serializedData(componentData.size() + 1);
    std::memcpy(serializedData.data(), componentData.c_str(), componentData.size() + 1);
    message.components.push_back({componentType, serializedData});
}

void addComponentToMessage(RTypeProtocol::NewEntityMessage& message,
                  RTypeProtocol::ComponentList     componentType,
                  const std::vector<char>&         componentData) {
    message.components.push_back({componentType, componentData});
}