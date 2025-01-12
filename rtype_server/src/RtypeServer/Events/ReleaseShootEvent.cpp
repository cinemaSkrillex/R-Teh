/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** ReleaseShootEvent
*/

#include "../include/RtypeServer/Events/ReleaseShootEvent.hpp"

#include "RtypeServer.hpp"

void ReleaseShootEvent::shootMiddleBullet(const std::array<char, 800>&   buffer,
                                          const asio::ip::udp::endpoint& client, Player& player,
                                          RtypeServer* server) {
    auto               gameInstance    = server->getGameInstance();
    const sf::Vector2f bullet_position = player.getPosition() + sf::Vector2f(32.5f, 7.5f);
    auto               bullet =
        gameInstance->addAndGetBullet(bullet_position, {1, 0}, 500, "mid_bullet", 15.f, 20);

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
    addCollisionComponentToMessage(bulletMessage, bounds, id, isColliding, type);
    // Serialize auto destructible component
    addComponentToMessage(bulletMessage, RTypeProtocol::ComponentList::AUTO_DESTRUCTIBLE, 2.f);

    // Serialize drawable component
    addComponentToMessage(bulletMessage, RTypeProtocol::ComponentList::DRAWABLE, true);

    // Serialize sprite component
    std::string       sprite = "mid_bullet";
    std::vector<char> spriteData(sprite.begin(), sprite.end());
    addComponentToMessage(bulletMessage, RTypeProtocol::ComponentList::SPRITE, spriteData);

    std::array<char, 800> serializedEventMessage = RTypeProtocol::serialize<800>(bulletMessage);
    broadcastAllReliable(serializedEventMessage, server);
}

void ReleaseShootEvent::shootBigBullet(const std::array<char, 800>&   buffer,
                                       const asio::ip::udp::endpoint& client, Player& player,
                                       RtypeServer* server) {
    auto               gameInstance    = server->getGameInstance();
    const sf::Vector2f bullet_position = player.getPosition() + sf::Vector2f(32.5f, 7.5f);
    auto               bullet =
        gameInstance->addAndGetBullet(bullet_position, {1, 0}, 500, "big_bullet", 25.f, 50);

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
    addCollisionComponentToMessage(bulletMessage, bounds, id, isColliding, type);
    // Serialize auto destructible component
    addComponentToMessage(bulletMessage, RTypeProtocol::ComponentList::AUTO_DESTRUCTIBLE, 2.f);

    // Serialize drawable component
    addComponentToMessage(bulletMessage, RTypeProtocol::ComponentList::DRAWABLE, true);

    // Serialize sprite component
    std::string       sprite = "big_bullet";
    std::vector<char> spriteData(sprite.begin(), sprite.end());
    addComponentToMessage(bulletMessage, RTypeProtocol::ComponentList::SPRITE, spriteData);

    std::array<char, 800> serializedEventMessage = RTypeProtocol::serialize<800>(bulletMessage);
    broadcastAllReliable(serializedEventMessage, server);
}

void ReleaseShootEvent::execute(const std::array<char, 800>&   buffer,
                                const asio::ip::udp::endpoint& client, Player& player,
                                RtypeServer* server) {
    auto  gameInstance = server->getGameInstance();
    auto* container =
        gameInstance->getRegistry()->get_component<RealEngine::NetvarContainer>(player.getEntity());
    auto* isHolding = container->getNetvar("isHolding");
    auto* holdTime  = container->getNetvar("holdTime");

    if (!std::any_cast<bool>(isHolding->value)) {
        return;
    }

    if (std::any_cast<float>(holdTime->value) > 0.75f) {
        shootBigBullet(buffer, client, player, server);
    } else {
        shootMiddleBullet(buffer, client, player, server);
    }

    isHolding->value = false;
    holdTime->value  = 0.f;
}
