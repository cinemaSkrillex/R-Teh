/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** HoldShootEvent
*/

#include "../include/RtypeServer/Events/HoldShootEvent.hpp"

#include "RtypeServer.hpp"

void HoldShootEvent::execute(const std::array<char, 800>&   buffer,
                             const asio::ip::udp::endpoint& client, Player& player,
                             RtypeServer* server) {
    auto gameInstance = server->getGameInstance();
    if (gameInstance == nullptr) return;
    auto* container =
        gameInstance->getRegistry()->get_component<RealEngine::NetvarContainer>(player.getEntity());
    auto* isHolding = container->getNetvar("isHolding");

    isHolding->value = true;
}
