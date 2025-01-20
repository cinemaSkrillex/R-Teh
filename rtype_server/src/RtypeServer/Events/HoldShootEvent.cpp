/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** HoldShootEvent
*/

#include "../include/RtypeServer/Events/HoldShootEvent.hpp"

#include "RtypeServer.hpp"

void HoldShootEvent::execute(const std::array<char, 800>&   buffer,
                             const asio::ip::udp::endpoint& client, ServerPlayer& player,
                             RtypeServer* server) {
    auto gameInstance = server->getGameInstance();
    if (gameInstance == nullptr) return;
    auto* container =
        gameInstance->getRegistry()->get_component<RealEngine::NetvarContainer>(player.getEntity());
    if (!container) {
        return;
    }
    auto* isHolding = container->getNetvar("isHolding");
    if (!isHolding) {
        return;
    }

    isHolding->value = true;
}
