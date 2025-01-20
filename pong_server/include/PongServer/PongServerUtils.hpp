/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** PongServerUtils.hpp
*/

#ifndef PONGSERVERUTILS_HPP_
#define PONGSERVERUTILS_HPP_

#include "shared/PongServerProtocol.hpp"

template <typename T>
void addComponentToMessage(PongProtocol::NewEntityMessage& message,
                           PongProtocol::ComponentList componentType, const T& componentData) {
    std::vector<char> serializedData(sizeof(T));
    std::memcpy(serializedData.data(), &componentData, sizeof(T));
    message.components.push_back({componentType, serializedData});
}

#endif /* !PONGSERVERUTILS_HPP_ */
