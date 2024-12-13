/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RtypeEvent
*/

#include "../../include/RtypeServer.hpp"

void RtypeServer::runEvent(const std::unordered_map<std::string, std::string>& parsed_data,
                           asio::ip::udp::endpoint& client, Player& player) {
    if (parsed_data.at("Event") == "Shoot") {
        std::cout << "Shoot event" << std::endl;
        _game_instance->addBullet(player.getPosition(), {1, 0}, 500);
        // TODO: send event to all players that entity was created with it's params (make it simple so reusable with mobs)
    }
}