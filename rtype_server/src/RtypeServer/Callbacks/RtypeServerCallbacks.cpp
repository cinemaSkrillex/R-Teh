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
        // new test
        auto player = playerInitializer->initializePlayer(sender);
        // playerInitializer->sendNewClientMessage(sender, player);
        // playerInitializer->sendSynchronizeMessage(sender, player);
        _players[sender] = player;
        notifyCurrentSceneOfNewClient(sender);

        // Notify the current scene about the new client

        // old  code
        //  auto player = playerInitializer->initializePlayer(sender);
        //  playerInitializer->sendNewClientMessage(sender, player);
        //  playerInitializer->sendSynchronizeMessage(sender, player);
        //  std::cout << "New client connected: " << sender << std::endl;
        //  std::cout << "New client connected: " << sender << std::endl;
        //  // Send all the entities to the new client, so it can synchronize and move
        //  mapInitializer->initializeMap(sender);
        //  mobInitializer->initializeMobs(sender);

        // // Send all the mobs to the new client (from SimpleMobs Class)
        // mobInitializer->initializeMobs(sender);
        // std::cout << "Mobs initialized" << std::endl;
        // _players[sender] = player;
        // updateScene();

        // send a ping to the new client
        RTypeProtocol::BaseMessage pingMessage = {};
        pingMessage.message_type               = RTypeProtocol::PING;
        pingMessage.uuid                       = 0;
        _server->send_reliable_packet(RTypeProtocol::serialize<800>(pingMessage), sender);
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
