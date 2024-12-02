/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** TCPPacketUtils.hpp
*/

#ifndef TCPPACKETUTILS_HPP
#define TCPPACKETUTILS_HPP

#include <algorithm>
#include <vector>

#include "PacketUtils.hpp"

// Structure for the TCP binary protocol
struct TCPPacket {
    Flags                   flag;
    std::vector<char>       data;
    asio::ip::tcp::endpoint endpoint;

    bool operator==(const TCPPacket& other) const {
        return flag == other.flag && data == other.data && endpoint == other.endpoint;
    }
};

// Serialize the packet before sending
inline std::vector<char> serializePacket(const TCPPacket& pkt) {
    std::vector<char> buffer;

    // Add flag
    buffer.push_back(static_cast<char>(pkt.flag));

    // Add endpoint (IP address and port)
    asio::ip::address_v4::bytes_type ip_bytes = pkt.endpoint.address().to_v4().to_bytes();
    buffer.insert(buffer.end(), ip_bytes.begin(), ip_bytes.end());

    uint16_t port = pkt.endpoint.port();
    buffer.push_back(static_cast<char>((port >> 8) & 0xFF));
    buffer.push_back(static_cast<char>(port & 0xFF));

    // Add data
    buffer.insert(buffer.end(), pkt.data.begin(), pkt.data.end());

    return buffer;
}

// Deserialize the packet after receiving
inline TCPPacket deserializePacket(const std::vector<char>& buffer) {
    TCPPacket pkt;

    // Extract flag
    pkt.flag = static_cast<Flags>(buffer[0]);

    // Extract endpoint (IP address and port)
    asio::ip::address_v4::bytes_type ip_bytes;
    std::memcpy(ip_bytes.data(), buffer.data() + 1, ip_bytes.size());
    asio::ip::address_v4 ip_address(ip_bytes);

    uint16_t port = (static_cast<uint8_t>(buffer[5]) << 8) | static_cast<uint8_t>(buffer[6]);
    pkt.endpoint  = asio::ip::tcp::endpoint(ip_address, port);

    // Extract data
    pkt.data.assign(buffer.begin() + 7, buffer.end());

    return pkt;
}

#endif  // TCPPACKETUTILS_HPP
