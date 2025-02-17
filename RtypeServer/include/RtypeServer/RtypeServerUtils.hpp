/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** RtypeServerUtils
*/

#ifndef RTYPESERVERUTILS_HPP_
#define RTYPESERVERUTILS_HPP_

#include "RtypeServerProtocol.hpp"

template <typename T>
void addComponentToMessage(RTypeProtocol::NewEntityMessage& message,
                           RTypeProtocol::ComponentList componentType, const T& componentData) {
    std::vector<char> serializedData(sizeof(T));
    std::memcpy(serializedData.data(), &componentData, sizeof(T));
    message.components.push_back({componentType, serializedData});
}

void addComponentToMessage(RTypeProtocol::NewEntityMessage& message,
                           RTypeProtocol::ComponentList     componentType,
                           const std::string&               componentData);

void addComponentToMessage(RTypeProtocol::NewEntityMessage& message,
                           RTypeProtocol::ComponentList     componentType,
                           const std::vector<char>&         componentData);

void addCollisionComponentToMessage(RTypeProtocol::NewEntityMessage& message,
                                    const sf::FloatRect& bounds, const std::string& id,
                                    bool isColliding, RealEngine::CollisionType type);
#endif /* !RTYPESERVERUTILS_HPP_ */
