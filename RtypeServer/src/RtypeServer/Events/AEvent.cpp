/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** AEvent
*/

#include "AEvent.hpp"

#include "RtypeServer.hpp"

void AEvent::broadcastAllUnreliable(const std::array<char, 800>& message, RtypeServer* server) {
    for (const auto& client : server->getServer()->getClients()) {
        server->getServer()->sendUnreliablePacket(message, client);
    }
}

void AEvent::broadcastAllReliable(const std::array<char, 800>& message, RtypeServer* server) {
    for (const auto& client : server->getServer()->getClients()) {
        server->getServer()->sendReliablePacket(message, client);
    }
}

std::array<char, 800> AEvent::createBulletMessage(long int id, sf::Vector2f position,
                                                  std::string          sprite_name,
                                                  RealEngine::Velocity velocity) {
    RTypeProtocol::NewEntityMessage bulletMessage;
    bulletMessage.messageType = RTypeProtocol::MessageType::NEW_ENTITY;
    bulletMessage.uuid        = id;

    // Serialize position component
    addComponentToMessage(bulletMessage, RTypeProtocol::ComponentList::POSITION, position);

    // Serialize velocity component
    addComponentToMessage(bulletMessage, RTypeProtocol::ComponentList::VELOCITY, velocity);

    // Serialize collision component
    sf::FloatRect             bounds      = {-1, -1, -1, -1};
    std::string               collisionId = "bullet";
    bool                      isColliding = false;
    RealEngine::CollisionType type        = RealEngine::CollisionType::OTHER;
    addCollisionComponentToMessage(bulletMessage, bounds, collisionId, isColliding, type);
    // Serialize auto destructible component
    addComponentToMessage(bulletMessage, RTypeProtocol::ComponentList::AUTO_DESTRUCTIBLE, 5.f);

    // Serialize drawable component
    addComponentToMessage(bulletMessage, RTypeProtocol::ComponentList::DRAWABLE, true);

    // Serialize sprite component
    std::vector<char> spriteData(sprite_name.begin(), sprite_name.end());
    addComponentToMessage(bulletMessage, RTypeProtocol::ComponentList::SPRITE, spriteData);

    // std::array<char, 800> serializedEventMessage = RTypeProtocol::serialize<800>(bulletMessage);
    return RTypeProtocol::serialize<800>(bulletMessage);
}