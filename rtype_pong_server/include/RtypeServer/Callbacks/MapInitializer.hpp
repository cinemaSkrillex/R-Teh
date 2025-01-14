/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** MapInitializer
*/

#ifndef MAPINITIALIZER_HPP_
#define MAPINITIALIZER_HPP_

#include "RtypeServer.hpp"
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
    void sendEntities(const std::vector<T>& entities, const std::string& type,
                      const asio::ip::udp::endpoint& sender) {
        constexpr size_t                   BATCH_SIZE = 25;
        std::vector<std::array<char, 800>> batchMessages;
        batchMessages.reserve(BATCH_SIZE);

        for (const auto& entity : entities) {
            if (type == "block") {
                processBlock(entity, batchMessages);
                if (batchMessages.size() == BATCH_SIZE) {
                    processBatchMessages(batchMessages, type, sender);
                }
            }
            processBatchMessages(batchMessages, type, sender);
        }
    }

    void processBatchMessages(const std::vector<std::array<char, 800>>& batchMessages,
                              const std::string& type, const asio::ip::udp::endpoint& sender) {
        for (const auto& message : batchMessages) {
            for (const auto& client : _UdpServer->getClients()) {
                _UdpServer->send_reliable_packet(message, client);
            }
        }
    }

    void processBlock(const std::shared_ptr<rtype::Block>& block,
                      std::vector<std::array<char, 800>>&  batchMessages);
};

#endif /* !MAPINITIALIZER_HPP_ */
