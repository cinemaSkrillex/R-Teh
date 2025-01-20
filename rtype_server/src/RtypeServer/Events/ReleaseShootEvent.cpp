/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** ReleaseShootEvent
*/

#include "../include/RtypeServer/Events/ReleaseShootEvent.hpp"

#include "RtypeServer.hpp"

void ReleaseShootEvent::shootMiddleBullet(const std::array<char, 800>&   buffer,
                                          const asio::ip::udp::endpoint& client,
                                          ServerPlayer& player, RtypeServer* server) {
    auto               gameInstance    = server->getGameInstance();
    const sf::Vector2f bullet_position = player.getPosition() + sf::Vector2f(32.5f, 7.5f);
    float              bulletDamage    = 10.f;
    try {
        bulletDamage = std::any_cast<float>(player.getNetvar("shootDamage")->value);
    } catch (const std::bad_any_cast& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return;
    }
    float bulletSpeed = 300.f;
    auto  bullet      = gameInstance->addAndGetBullet(bullet_position, bulletSpeed, "mid_bullet",
                                                      bulletDamage * 1.5f, int(bulletDamage) * 2,
                                                      *(player.getEntity()));
    if (!bullet) {
        return;
    }

    std::array<char, 800> serializedEventMessage = createBulletMessage(
        *bullet, bullet_position, "mid_bullet", {bulletSpeed, 0, {bulletSpeed, bulletSpeed}, 0.f});
    broadcastAllReliable(serializedEventMessage, server);
}

void ReleaseShootEvent::shootBigBullet(const std::array<char, 800>&   buffer,
                                       const asio::ip::udp::endpoint& client, ServerPlayer& player,
                                       RtypeServer* server) {
    auto               gameInstance    = server->getGameInstance();
    const sf::Vector2f bullet_position = player.getPosition() + sf::Vector2f(32.5f, 7.5f);
    float              bulletDamage    = 10.f;
    try {
        bulletDamage = std::any_cast<float>(player.getNetvar("shootDamage")->value);
    } catch (const std::bad_any_cast& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    float bulletSpeed = 400.f;
    auto  bullet      = gameInstance->addAndGetBullet(bullet_position, bulletSpeed, "big_bullet",
                                                      bulletDamage * 2.5f, int(bulletDamage) * 5,
                                                      *(player.getEntity()));

    std::array<char, 800> serializedEventMessage = createBulletMessage(
        *bullet, bullet_position, "big_bullet", {bulletSpeed, 0, {bulletSpeed, bulletSpeed}, 0.f});
    broadcastAllReliable(serializedEventMessage, server);
}

void ReleaseShootEvent::execute(const std::array<char, 800>&   buffer,
                                const asio::ip::udp::endpoint& client, ServerPlayer& player,
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
