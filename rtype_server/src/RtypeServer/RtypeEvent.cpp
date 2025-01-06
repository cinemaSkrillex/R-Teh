/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RtypeEvent
*/

#include "../../include/RtypeServer/RtypeServer.hpp"

void RtypeServer::runEvent(const std::array<char, 800>& buffer, asio::ip::udp::endpoint& client,
                           Player& player) {
    RTypeProtocol::BaseMessage baseMessage = RTypeProtocol::deserialize<800>(buffer);

    if (baseMessage.message_type == RTypeProtocol::MessageType::SHOOT_EVENT) {
        // check player entity netvar shootCooldown
        // auto* component = registry.get_component<Component>(entity);
        auto* netvar =
            _game_instance->getRegistry()->get_component<RealEngine::Netvar>(player.getEntity());
        if (netvar == nullptr) {
            return;
        }
        if (std::any_cast<float>(netvar->value) > 0) {
            return;
        }
        netvar->value = 0.5f;

        const sf::Vector2f bullet_position = player.getPosition() + sf::Vector2f(32.5f, 7.5f);

        auto bullet = _game_instance->addAndGetBullet(bullet_position, {1, 0}, 500);

        RTypeProtocol::NewEntityMessage bulletMessage;

        bulletMessage.message_type = RTypeProtocol::MessageType::NEW_ENTITY;
        bulletMessage.uuid         = *bullet;
        // eventMessage.event_type   = RTypeProtocol::EventType::SHOOT;

        // Serialize position component
        std::vector<char> positionData(sizeof(sf::Vector2f));
        std::memcpy(positionData.data(), &bullet_position, sizeof(sf::Vector2f));
        bulletMessage.components.push_back({RTypeProtocol::ComponentList::POSITION, positionData});

        // Serialize velocity component
        RealEngine::Velocity velocity = {500.f, 0.f, {500.f, 500.f}, 0.f};
        std::vector<char>    velocityData(sizeof(RealEngine::Velocity));
        std::memcpy(velocityData.data(), &velocity, sizeof(RealEngine::Velocity));
        bulletMessage.components.push_back({RTypeProtocol::ComponentList::VELOCITY, velocityData});

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
        bulletMessage.components.push_back(
            {RTypeProtocol::ComponentList::COLLISION, collisionData});

        // Serialize auto destructible component
        bulletMessage = addAutoDestructibleComponent(bulletMessage, 5.f);

        // Serialize drawable component
        bulletMessage = addDrawableComponent(bulletMessage);

        // Serialize sprite component
        std::string       sprite = "bullet";
        std::vector<char> spriteData(sprite.begin(), sprite.end());
        bulletMessage.components.push_back({RTypeProtocol::ComponentList::SPRITE, spriteData});

        std::array<char, 800> serializedEventMessage = RTypeProtocol::serialize<800>(bulletMessage);
        broadCastAll(serializedEventMessage);

        return;
    }
    std::cout << "Unknown message type: " << baseMessage.message_type << std::endl;
}