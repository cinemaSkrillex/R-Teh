/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** RtypeServerUtils
*/

#ifndef RTYPESERVERUTILS_HPP_
#define RTYPESERVERUTILS_HPP_

#include "RtypeServerProtocol.hpp"

RTypeProtocol::NewEntityMessage addDrawableComponent(RTypeProtocol::NewEntityMessage& message);
RTypeProtocol::NewEntityMessage addAutoDestructibleComponent(
    RTypeProtocol::NewEntityMessage& message, float autoDestructibleTime);

template <typename T>
void addComponent(RTypeProtocol::NewEntityMessage& message,
                  RTypeProtocol::ComponentList componentType, const T& componentData) {
    std::vector<char> serializedData(sizeof(T));
    std::memcpy(serializedData.data(), &componentData, sizeof(T));
    message.components.push_back({componentType, serializedData});
}

void addComponent(RTypeProtocol::NewEntityMessage& message,
                  RTypeProtocol::ComponentList componentType, const std::string& componentData);

void addComponent(RTypeProtocol::NewEntityMessage& message,
                  RTypeProtocol::ComponentList     componentType,
                  const std::vector<char>&         componentData);

#endif /* !RTYPESERVERUTILS_HPP_ */
