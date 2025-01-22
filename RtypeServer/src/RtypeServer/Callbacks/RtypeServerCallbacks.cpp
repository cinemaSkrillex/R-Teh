/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RtypeServerCallbacks
*/

#include "../../include/RtypeServer/RtypeServer.hpp"

void RtypeServer::initCallbacks() {
    auto playerInitializer = std::make_shared<PlayerInitializer>(this);
    _server->setNewClientCallback([this, playerInitializer](const asio::ip::udp::endpoint& sender) {
        auto player = playerInitializer->initializePlayer(sender);

        _players[sender] = player;
        notifyCurrentSceneOfNewClient(sender);
    });
}
