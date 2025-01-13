/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RtypeServerCallbacks
*/

#include "../../include/RtypeServer/RtypeServer.hpp"

void RtypeServer::initCallbacks() {
    auto playerInitializer = std::make_shared<PlayerInitializer>(this);
    auto mobInitializer    = std::make_shared<MobInitializer>(_game_instance, _server);
    _server->setNewClientCallback(
        [this, playerInitializer, mobInitializer](const asio::ip::udp::endpoint& sender) {
            auto player = playerInitializer->initializePlayer(sender);

            // Send all the mobs to the new client (from SimpleMobs Class)
            mobInitializer->initializeMobs(sender);

            _players[sender] = player;
        });
}
