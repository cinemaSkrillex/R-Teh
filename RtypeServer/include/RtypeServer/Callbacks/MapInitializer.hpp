/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** MapInitializer
*/

#ifndef MAPINITIALIZER_HPP_
#define MAPINITIALIZER_HPP_

#include "../RtypeServer.hpp"
#include "ServerConfig.hpp"
#include "UDPServer.hpp"

class MapInitializer {
   public:
    explicit MapInitializer(std::shared_ptr<GameInstance> gameInstance,
                            std::shared_ptr<UDPServer> UdpServer, const ServerConfig& serverConfig)
        : _gameInstance(gameInstance), _UdpServer(UdpServer), _serverConfig(serverConfig) {}

    void initializeMap(const asio::ip::udp::endpoint& sender);

   private:
    std::shared_ptr<GameInstance> _gameInstance;
    std::shared_ptr<UDPServer>    _UdpServer;
    ServerConfig                  _serverConfig;

    template <typename T>
    void sendEntities(const std::unordered_map<std::size_t, std::shared_ptr<T>>& entities,
                      const std::string& type, const asio::ip::udp::endpoint& sender) {
        for (const auto& [key, entity] : entities) {
            if (type == "block") {
                std::array<char, 800> message;
                processBlock(entity, message);
                processMessage(message, sender);
            }
        }
    }

    void processMessage(const std::array<char, 800>&   message,
                        const asio::ip::udp::endpoint& sender) {
        for (const auto& client : _UdpServer->getClients()) {
            _UdpServer->sendReliablePacket(message, client);
        }
    }

    void                      processBlock(const std::shared_ptr<rtype::BaseBlock>& block,
                                           std::array<char, 800>&                   message);
    RTypeProtocol::MapMessage createMapMessage(const std::shared_ptr<ServerMap>& GameMap);
};

#endif /* !MAPINITIALIZER_HPP_ */
