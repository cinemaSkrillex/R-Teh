/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** PlayerInitializer
*/

#ifndef PLAYERINITIALIZER_HPP_
#define PLAYERINITIALIZER_HPP_

#include "RtypeServer.hpp"

class PlayerInitializer {
   public:
    explicit PlayerInitializer(GameInstance* gameInstance, UDPServer* UDPServer)
        : _gameInstance(gameInstance), _UdpServer(UDPServer) {}

    Player initializePlayer(const asio::ip::udp::endpoint& sender);

   private:
    long getCurrentTime() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::steady_clock::now().time_since_epoch())
            .count();
    }

    void sendNewClientMessages(const asio::ip::udp::endpoint& sender, int playerEntity, float x,
                               float y) {
        // Send player initialization messages
    }

    GameInstance* _gameInstance;
    UDPServer*    _UdpServer;
};

#endif /* !PLAYERINITIALIZER_HPP_ */
