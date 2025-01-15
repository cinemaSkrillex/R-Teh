/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RtypeServerCallbacks
*/

#include "../../include/RtypeServer/RtypeServer.hpp"

void RtypeServer::initCallbacks() {
    // auto mapInitializer = std::make_shared<MapInitializer>(_game_instance, _server,
    // _server_config);
    auto playerInitializer = std::make_shared<PlayerInitializer>(this);
    auto mobInitializer    = std::make_shared<MobInitializer>(_game_instance, _server);
    _server->setNewClientCallback(
        [this, playerInitializer, mobInitializer](const asio::ip::udp::endpoint& sender) {
            auto player = playerInitializer->initializePlayer(sender);

            // std::cout << "New client connected: " << sender << std::endl;
            // // Send all the entities to the new client, so it can synchronize and move
            // mapInitializer->initializeMap(sender);
            // mobInitializer->initializeMobs(sender);

            // // Send all the mobs to the new client (from SimpleMobs Class)
            // mobInitializer->initializeMobs(sender);
            // std::cout << "Mobs initialized" << std::endl;
            _players[sender] = player;
        });
}
// new client callback
//  auto playerInitializer = std::make_shared<PlayerInitializer>(this);
//  auto mobInitializer    = std::make_shared<MobInitializer>(_game_instance, _server);
//  _server->setNewClientCallback([this, playerInitializer](const asio::ip::udp::endpoint&
//  sender) {
//      auto player = playerInitializer->initializePlayer(sender);

//     _players[sender] = player;
// });
