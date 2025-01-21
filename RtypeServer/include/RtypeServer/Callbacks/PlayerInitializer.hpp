/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** PlayerInitializer
*/

#ifndef PLAYERINITIALIZER_HPP_
#define PLAYERINITIALIZER_HPP_

#include <memory>

#include "../RtypeServer.hpp"
#include "UDPServer.hpp"

class ServerPlayer;
class RtypeServer;
class GameInstance;
class UDPServer;

class PlayerInitializer {
   public:
    explicit PlayerInitializer(RtypeServer* server) : _server(server) {}

    ServerPlayer initializePlayer(const asio::ip::udp::endpoint& sender);

    void sendNewClientMessage(const asio::ip::udp::endpoint& sender, const ServerPlayer& player);

    void sendSynchronizeMessage(const asio::ip::udp::endpoint& sender, const ServerPlayer& player);

   private:
    long getCurrentTime() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::steady_clock::now().time_since_epoch())
            .count();
    }
    void sendNewClientMessages(const asio::ip::udp::endpoint& sender, long playerEntity,
                               sf::Vector2f player_start_pos, long timestamp);
    void sendSynchronizeMessage(const asio::ip::udp::endpoint& sender, long playerEntity,
                                const sf::Vector2f& player_start_position, long timestamp);

    long getElapsedTime() const;

    RtypeServer* _server;
};

#endif /* !PLAYERINITIALIZER_HPP_ */
