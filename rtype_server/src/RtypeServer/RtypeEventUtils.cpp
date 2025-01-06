/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** RtypeEventUtils
*/

#include "../../include/RtypeServer/RtypeServerUtils.hpp"

RTypeProtocol::NewEntityMessage addDrawableComponent(RTypeProtocol::NewEntityMessage& message) {
    // Serialize drawable component
    bool              drawable = true;
    std::vector<char> drawableData(sizeof(bool));
    std::memcpy(drawableData.data(), &drawable, sizeof(bool));
    message.components.push_back({RTypeProtocol::ComponentList::DRAWABLE, drawableData});

    return message;
}

RTypeProtocol::NewEntityMessage addAutoDestructibleComponent(
    RTypeProtocol::NewEntityMessage& message, float autoDestructibleTime) {
    // Serialize auto destructible component
    float             autoDestructible = autoDestructibleTime;
    std::vector<char> autoDestructibleData(sizeof(int));
    std::memcpy(autoDestructibleData.data(), &autoDestructible, sizeof(int));
    message.components.push_back(
        {RTypeProtocol::ComponentList::AUTO_DESTRUCTIBLE, autoDestructibleData});

    return message;
}

void addComponent(RTypeProtocol::NewEntityMessage& message,
                  RTypeProtocol::ComponentList componentType, const std::string& componentData) {
    std::vector<char> serializedData(componentData.size() + 1);
    std::memcpy(serializedData.data(), componentData.c_str(), componentData.size() + 1);
    message.components.push_back({componentType, serializedData});
}

void addComponent(RTypeProtocol::NewEntityMessage& message,
                  RTypeProtocol::ComponentList     componentType,
                  const std::vector<char>&         componentData) {
    message.components.push_back({componentType, componentData});
}