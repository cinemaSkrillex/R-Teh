/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** MobInitializer
*/

#ifndef MOBINITIALIZER_HPP_
#define MOBINITIALIZER_HPP_

#include "RtypeServer.hpp"

class GameInstance;
class UDPServer;

class MobInitializer {
   public:
    explicit MobInitializer(std::shared_ptr<GameInstance> gameInstance,
                            std::shared_ptr<UDPServer>    UdpServer)
        : _gameInstance(gameInstance), _UdpServer(UdpServer) {}

    void initializeMobs(const asio::ip::udp::endpoint& sender);

   private:
    std::shared_ptr<GameInstance> _gameInstance;
    std::shared_ptr<UDPServer>    _UdpServer;
};

#endif /* !MOBINITIALIZER_HPP_ */
