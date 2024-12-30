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

class INetwork {
   public:
    virtual ~INetwork() = default;

    // Send methods
    virtual void send_unreliable_packet(const std::array<char, BUFFER_SIZE>& message,
                                        const asio::ip::udp::endpoint&       endpoint) = 0;
    virtual void send_reliable_packet(const std::array<char, BUFFER_SIZE>& message,
                                      const asio::ip::udp::endpoint&       endpoint)   = 0;

    // Receive methods
    virtual std::vector<std::array<char, BUFFER_SIZE>> get_unreliable_messages_from_endpoint(
        const asio::ip::udp::endpoint& endpoint) = 0;

    virtual std::vector<std::array<char, BUFFER_SIZE>> get_reliable_messages_from_endpoint(
        const asio::ip::udp::endpoint& endpoint) = 0;

    virtual std::array<char, BUFFER_SIZE> get_last_unreliable_packet_data() = 0;
    virtual std::array<char, BUFFER_SIZE> get_last_reliable_packet_data()   = 0;
};

#endif /* !INETWORK_HPP_ */
