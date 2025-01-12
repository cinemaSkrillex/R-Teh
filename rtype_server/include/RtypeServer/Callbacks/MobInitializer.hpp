/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** MobInitializer
*/

#ifndef MOBINITIALIZER_HPP_
#define MOBINITIALIZER_HPP_

#include "RtypeServer.hpp"

class MobInitializer {
   public:
    explicit MobInitializer(GameInstance* gameInstance, UDPServer* UdpServer)
        : _gameInstance(gameInstance), _UdpServer(UdpServer) {}

    void initializeMobs(const asio::ip::udp::endpoint& sender);

   private:
    GameInstance* _gameInstance;
    UDPServer*    _UdpServer;
};

#endif /* !MOBINITIALIZER_HPP_ */
