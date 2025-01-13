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
        server->getServer()->send_unreliable_packet(message, client);
    }
}

void AEvent::broadcastAllReliable(const std::array<char, 800>& message, RtypeServer* server) {
    for (const auto& client : server->getServer()->getClients()) {
        server->getServer()->send_reliable_packet(message, client);
    }
}