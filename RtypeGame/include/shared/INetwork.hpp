/*
** EPITECH PROJECT, 2024
** r-teh-mega-backup
** File description:
** INetwork
*/

#ifndef INETWORK_HPP_
#define INETWORK_HPP_

#include <array>
#include <asio.hpp>
#include <string>
#include <vector>

#include "PacketUtils.hpp"

template <std::size_t BUFFER_SIZE>
class INetwork {
   public:
    virtual ~INetwork() = default;

    // Send methods
    virtual void sendUnreliablePacket(const std::array<char, BUFFER_SIZE>& message,
                                      const asio::ip::udp::endpoint&       endpoint) = 0;
    virtual void sendReliablePacket(const std::array<char, BUFFER_SIZE>& message,
                                    const asio::ip::udp::endpoint&       endpoint)   = 0;

    // Receive methods
    virtual std::vector<std::array<char, BUFFER_SIZE>> getUnreliableMessagesFromEndpoint(
        const asio::ip::udp::endpoint& endpoint) = 0;

    virtual std::vector<std::array<char, BUFFER_SIZE>> getReliableMessagesFromEndpoint(
        const asio::ip::udp::endpoint& endpoint) = 0;

    virtual std::array<char, BUFFER_SIZE> getLastUnreliablePacketData() = 0;
    virtual std::array<char, BUFFER_SIZE> getLastReliablePacketData()   = 0;
};

#endif /* !INETWORK_HPP_ */
