/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** RtypeEventType
*/

#include "../../include/RtypeServer/RtypeServer.hpp"

void RtypeServer::shootEvent(const std::array<char, 800>&   buffer,
                             const asio::ip::udp::endpoint& client, Player& player) {
    auto* netvar =
        _game_instance->getRegistry()->get_component<RealEngine::Netvar>(player.getEntity());
    if (netvar == nullptr || std::any_cast<float>(netvar->value) > 0) {
        return;
    }
    netvar->value = 0.5f;

    const sf::Vector2f bullet_position = player.getPosition() + sf::Vector2f(32.5f, 7.5f);
    auto               bullet = _game_instance->addAndGetBullet(bullet_position, {1, 0}, 500);

    RTypeProtocol::NewEntityMessage bulletMessage;
    bulletMessage.message_type = RTypeProtocol::MessageType::NEW_ENTITY;
    bulletMessage.uuid         = *bullet;

    // Serialize position component
    addComponentToMessage(bulletMessage, RTypeProtocol::ComponentList::POSITION, bullet_position);

    // Serialize velocity component
    RealEngine::Velocity velocity = {500.f, 0.f, {500.f, 500.f}, 0.f};
    addComponentToMessage(bulletMessage, RTypeProtocol::ComponentList::VELOCITY, velocity);

    // Serialize collision component
    sf::FloatRect             bounds      = {0, 0, 16, 8};
    std::string               id          = "bullet";
    bool                      isColliding = false;
    RealEngine::CollisionType type        = RealEngine::CollisionType::OTHER;

    std::vector<char> collisionData(sizeof(bounds) + id.size() + 1 + sizeof(isColliding) +
                                    sizeof(type));
    char*             collisionPtr = collisionData.data();
    std::memcpy(collisionPtr, &bounds, sizeof(bounds));
    collisionPtr += sizeof(bounds);
    std::memcpy(collisionPtr, id.c_str(), id.size() + 1);
    collisionPtr += id.size() + 1;
    std::memcpy(collisionPtr, &isColliding, sizeof(isColliding));
    collisionPtr += sizeof(isColliding);
    std::memcpy(collisionPtr, &type, sizeof(type));
    bulletMessage.components.push_back({RTypeProtocol::ComponentList::COLLISION, collisionData});

    // Serialize auto destructible component
    addComponentToMessage(bulletMessage, RTypeProtocol::ComponentList::AUTO_DESTRUCTIBLE, 2.f);

    // Serialize drawable component
    addComponentToMessage(bulletMessage, RTypeProtocol::ComponentList::DRAWABLE, true);

    // Serialize sprite component
    std::string       sprite = "bullet";
    std::vector<char> spriteData(sprite.begin(), sprite.end());
    addComponentToMessage(bulletMessage, RTypeProtocol::ComponentList::SPRITE, spriteData);

    std::array<char, 800> serializedEventMessage = RTypeProtocol::serialize<800>(bulletMessage);
    broadcastAllReliable(serializedEventMessage);
}