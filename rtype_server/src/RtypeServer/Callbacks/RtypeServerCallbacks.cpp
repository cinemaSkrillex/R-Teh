/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RtypeServerCallbacks
*/

#include "../../include/RtypeServer/RtypeServer.hpp"

void RtypeServer::initCallbacks() {
    auto mapInitializer = std::make_shared<MapInitializer>(_game_instance, _server, _server_config);
    auto playerInitializer = std::make_shared<PlayerInitializer>(this);
    auto mobInitializer    = std::make_shared<MobInitializer>(_game_instance, _server);
    _server->setNewClientCallback([this, mapInitializer, playerInitializer,
                                   mobInitializer](const asio::ip::udp::endpoint& sender) {
        auto player = playerInitializer->initializePlayer(sender);

        // Send all the entities to the new client, so it can synchronize and move
        mapInitializer->initializeMap(sender);

        // init_callback_mobs(sender);
        mobInitializer->initializeMobs(sender);

        _players[sender] = player;
    });
}
