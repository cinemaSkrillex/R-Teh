// /*
// ** EPITECH PROJECT, 2024
// ** R-Teh
// ** File description:
// ** TCPPacketUtils.hpp
// */

#pragma once

#include <algorithm>
#include <asio.hpp>
#include <vector>

#include "PacketUtils.hpp"

enum class TCPFlags {
    CONTROL    = 0,
    DATA       = 1,
    HEARTBEAT  = 2,
    NEW_CLIENT = 3,
    TEST       = 4,
    IMAGE      = 5,
};

struct TCPPacket {
    int                     sequence_nb;
    int                     start_sequence_nb;
    int                     end_sequence_nb;
    int                     packet_size;
    TCPFlags                flag;
    asio::ip::tcp::endpoint endpoint;
    std::vector<char>       data;

    bool operator==(const TCPPacket& other) const {
        return sequence_nb == other.sequence_nb && start_sequence_nb == other.start_sequence_nb &&
               end_sequence_nb == other.end_sequence_nb && packet_size == other.packet_size &&
               flag == other.flag && data == other.data && endpoint == other.endpoint;
    }
};

namespace std {
template <>
struct hash<TCPPacket> {
    std::size_t operator()(const TCPPacket& pkt) const {
        return ((std::hash<int>()(pkt.sequence_nb) ^
                 (std::hash<int>()(pkt.start_sequence_nb) << 1)) >>
                1) ^
               (std::hash<int>()(pkt.end_sequence_nb) << 1);
    }
};
}  // namespace std

inline std::vector<char> serialize(const TCPPacket& pkt) {
    std::string endpoint_address = pkt.endpoint.address().to_string();
    uint16_t    endpoint_port    = pkt.endpoint.port();

    std::size_t total_size = sizeof(pkt.sequence_nb) + sizeof(pkt.start_sequence_nb) +
                             sizeof(pkt.end_sequence_nb) + sizeof(pkt.packet_size) +
                             sizeof(pkt.flag) + sizeof(uint16_t) + sizeof(std::size_t) +
                             endpoint_address.size() + pkt.data.size();

    std::vector<char> buffer(total_size);
    auto              it = buffer.begin();

    // Copy sequence_nb
    std::copy(reinterpret_cast<const char*>(&pkt.sequence_nb),
              reinterpret_cast<const char*>(&pkt.sequence_nb) + sizeof(pkt.sequence_nb), it);
    it += sizeof(pkt.sequence_nb);

    // Copy start_sequence_nb
    std::copy(reinterpret_cast<const char*>(&pkt.start_sequence_nb),
              reinterpret_cast<const char*>(&pkt.start_sequence_nb) + sizeof(pkt.start_sequence_nb),
              it);
    it += sizeof(pkt.start_sequence_nb);

    // Copy end_sequence_nb
    std::copy(reinterpret_cast<const char*>(&pkt.end_sequence_nb),
              reinterpret_cast<const char*>(&pkt.end_sequence_nb) + sizeof(pkt.end_sequence_nb),
              it);
    it += sizeof(pkt.end_sequence_nb);

    // Copy packet_size
    std::copy(reinterpret_cast<const char*>(&pkt.packet_size),
              reinterpret_cast<const char*>(&pkt.packet_size) + sizeof(pkt.packet_size), it);
    it += sizeof(pkt.packet_size);

    // Copy flag
    int flag = static_cast<int>(pkt.flag);
    std::copy(reinterpret_cast<const char*>(&flag),
              reinterpret_cast<const char*>(&flag) + sizeof(flag), it);
    it += sizeof(flag);

    // Copy endpoint address length and address
    std::size_t address_length = endpoint_address.size();
    std::copy(reinterpret_cast<const char*>(&address_length),
              reinterpret_cast<const char*>(&address_length) + sizeof(address_length), it);
    it += sizeof(address_length);

    std::copy(endpoint_address.begin(), endpoint_address.end(), it);
    it += endpoint_address.size();

    // Copy endpoint port
    std::copy(reinterpret_cast<const char*>(&endpoint_port),
              reinterpret_cast<const char*>(&endpoint_port) + sizeof(endpoint_port), it);
    it += sizeof(endpoint_port);

    // Copy data
    std::copy(pkt.data.begin(), pkt.data.end(), it);

    return buffer;
}

inline TCPPacket deserialize(const std::vector<char>& buffer) {
    TCPPacket pkt;
    auto      it = buffer.begin();

    // Extract sequence_nb
    std::copy(it, it + sizeof(pkt.sequence_nb), reinterpret_cast<char*>(&pkt.sequence_nb));
    it += sizeof(pkt.sequence_nb);

    // Extract start_sequence_nb
    std::copy(it, it + sizeof(pkt.start_sequence_nb),
              reinterpret_cast<char*>(&pkt.start_sequence_nb));
    it += sizeof(pkt.start_sequence_nb);

    // Extract end_sequence_nb
    std::copy(it, it + sizeof(pkt.end_sequence_nb), reinterpret_cast<char*>(&pkt.end_sequence_nb));
    it += sizeof(pkt.end_sequence_nb);

    // Extract packet_size
    std::copy(it, it + sizeof(pkt.packet_size), reinterpret_cast<char*>(&pkt.packet_size));
    it += sizeof(pkt.packet_size);

    // Extract flag
    int flag;
    std::copy(it, it + sizeof(flag), reinterpret_cast<char*>(&flag));
    pkt.flag = static_cast<TCPFlags>(flag);
    it += sizeof(flag);

    // Extract endpoint address length and address
    std::size_t address_length;
    std::copy(it, it + sizeof(address_length), reinterpret_cast<char*>(&address_length));
    it += sizeof(address_length);

    std::string endpoint_address(it, it + address_length);
    it += address_length;

    // Extract endpoint port
    uint16_t endpoint_port;
    std::copy(it, it + sizeof(endpoint_port), reinterpret_cast<char*>(&endpoint_port));
    it += sizeof(endpoint_port);

    pkt.endpoint =
        asio::ip::tcp::endpoint(asio::ip::address::from_string(endpoint_address), endpoint_port);

    // Extract data
    pkt.data.assign(it, buffer.end());

    return pkt;
}