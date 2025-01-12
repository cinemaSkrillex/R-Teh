/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** PlayerInitializer
*/

#include "PlayerInitializer.hpp"

Player PlayerInitializer::initializePlayer(const asio::ip::udp::endpoint& sender) {
    sf::Vector2f player_start_position = {100.0f, 100.0f};  // Example position
    auto         playerEntity          = _gameInstance->addAndGetPlayer(player_start_position);
    auto         player =
        Player(*playerEntity, getCurrentTime(), playerEntity, _gameInstance->getRegistry());

    sendNewClientMessages(sender, *playerEntity, player_start_position.x, player_start_position.y);
    return player;
}
