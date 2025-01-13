/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** RtypeEventUtils
*/

#include "../../include/RtypeServer/RtypeServerUtils.hpp"

void addComponentToMessage(RTypeProtocol::NewEntityMessage& message,
                           RTypeProtocol::ComponentList     componentType,
                           const std::string&               componentData) {
    std::vector<char> serializedData(componentData.size() + 1);
    std::memcpy(serializedData.data(), componentData.c_str(), componentData.size() + 1);
    message.components.push_back({componentType, serializedData});
}

void addComponentToMessage(RTypeProtocol::NewEntityMessage& message,
                           RTypeProtocol::ComponentList     componentType,
                           const std::vector<char>&         componentData) {
    message.components.push_back({componentType, componentData});
}

void addCollisionComponentToMessage(RTypeProtocol::NewEntityMessage& message,
                                    const sf::FloatRect& bounds, const std::string& id,
                                    bool isColliding, RealEngine::CollisionType type) {
    std::vector<char> collisionData(sizeof(bounds) + id.size() + 1 + sizeof(isColliding) +
                                    sizeof(type));
    char*             collisionPtr = collisionData.data();

    // Serialize bounds
    std::memcpy(collisionPtr, &bounds, sizeof(bounds));
    collisionPtr += sizeof(bounds);

    // Serialize id
    std::memcpy(collisionPtr, id.c_str(), id.size() + 1);
    collisionPtr += id.size() + 1;

    // Serialize isColliding
    std::memcpy(collisionPtr, &isColliding, sizeof(isColliding));
    collisionPtr += sizeof(isColliding);

    // Serialize collision type
    std::memcpy(collisionPtr, &type, sizeof(type));

    // Add the collision component to the message
    message.components.push_back({RTypeProtocol::ComponentList::COLLISION, collisionData});
}
