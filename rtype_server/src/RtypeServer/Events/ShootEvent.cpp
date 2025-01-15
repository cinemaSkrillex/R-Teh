/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** ShootEvent
*/

#include "../include/RtypeServer/Events/ShootEvent.hpp"

#include "RtypeServer.hpp"

void ShootEvent::execute(const std::array<char, 800>& buffer, const asio::ip::udp::endpoint& client,
                         Player& player, RtypeServer* server) {
    auto  game_instance = server->getGameInstance();
    auto* container     = game_instance->getRegistry()->get_component<RealEngine::NetvarContainer>(
        player.getEntity());
    if (!container) {
        return;
    }
    auto* netvar = container->getNetvar("shootCooldown");
    if (netvar == nullptr || std::any_cast<float>(netvar->value) > 0) {
        return;
    }
    netvar->value      = 0.5f;
    float bulletDamage = 10.f;
    try {
        bulletDamage = std::any_cast<float>(player.getNetvar("shootDamage")->value);
    } catch (const std::bad_any_cast& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    float              bulletSpeed     = 500.f;
    const sf::Vector2f bullet_position = player.getPosition() + sf::Vector2f(32.5f, 7.5f);
    auto               bullet =
        game_instance->addAndGetBullet(bullet_position, bulletSpeed, "bullet", bulletDamage,
                                       int(bulletDamage), *(player.getEntity()));

    std::array<char, 800> serializedEventMessage = createBulletMessage(
        *bullet, bullet_position, "bullet", {bulletSpeed, 0, {bulletSpeed, bulletSpeed}, 0.f});
    broadcastAllReliable(serializedEventMessage, server);
}