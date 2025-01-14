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

    std::array<char, 800> serializedEventMessage =
        createBulletMessage(*bullet, bullet_position, "mid_bullet");
    broadcastAllReliable(serializedEventMessage, server);
}

void ReleaseShootEvent::shootBigBullet(const std::array<char, 800>&   buffer,
                                       const asio::ip::udp::endpoint& client, Player& player,
                                       RtypeServer* server) {
    auto               gameInstance    = server->getGameInstance();
    const sf::Vector2f bullet_position = player.getPosition() + sf::Vector2f(32.5f, 7.5f);
    auto               bullet =
        gameInstance->addAndGetBullet(bullet_position, {1, 0}, 500, "big_bullet", 25.f, 50);

    std::array<char, 800> serializedEventMessage =
        createBulletMessage(*bullet, bullet_position, "big_bullet");
    broadcastAllReliable(serializedEventMessage, server);
}

void ReleaseShootEvent::execute(const std::array<char, 800>&   buffer,
                                const asio::ip::udp::endpoint& client, Player& player,
                                RtypeServer* server) {
    auto  gameInstance = server->getGameInstance();
    auto* container =
        gameInstance->getRegistry()->get_component<RealEngine::NetvarContainer>(player.getEntity());
    if (!container) {
        return;
    }
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
