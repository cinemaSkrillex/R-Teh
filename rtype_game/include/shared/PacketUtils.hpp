/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** PacketUtils.hpp
*/

#ifndef PACKETUTILS_HPP
#define PACKETUTILS_HPP

#include <algorithm>
#include <asio.hpp>
#include <vector>

#define SEQUENCE_TYPE std::uint32_t  // TODO change int to SEQUENCE_TYPE
#define BUFFER_SIZE 800

enum class Role { SERVER, CLIENT };

struct EndpointHash {
    std::size_t operator()(const asio::ip::udp::endpoint& endpoint) const {
        std::size_t h1 = std::hash<std::string>()(endpoint.address().to_string());
        std::size_t h2 = std::hash<unsigned short>()(endpoint.port());
        return h1 ^ (h2 << 1);  // Combine the hashes
    }
};

struct EndpointEqual {
    bool operator()(const asio::ip::udp::endpoint& lhs, const asio::ip::udp::endpoint& rhs) const {
        return lhs.address() == rhs.address() && lhs.port() == rhs.port();
    }
};

enum Flags {
    UNRELIABLE    = 0,
    RELIABLE      = 1,
    ACK           = 2,
    CONTROL       = 3,
    HEARTBEAT     = 4,
    DATA          = 5,
    RETRANSMITTED = 6,
    NEW_CLIENT    = 7,
    TEST          = 8,
};

struct packet {
    SEQUENCE_TYPE           sequence_nb;
    SEQUENCE_TYPE           start_sequence_nb;
    SEQUENCE_TYPE           end_sequence_nb;
    SEQUENCE_TYPE           packet_size;
    Flags                   flag;
    asio::ip::udp::endpoint endpoint;
    std::vector<char>       data;

    bool operator==(const packet& other) const {
        return sequence_nb == other.sequence_nb && start_sequence_nb == other.start_sequence_nb &&
               end_sequence_nb == other.end_sequence_nb && packet_size == other.packet_size &&
               flag == other.flag && data == other.data && endpoint == other.endpoint;
    }
};

namespace std {
template <>
struct hash<packet> {
    std::size_t operator()(const packet& pkt) const {
        return ((std::hash<int>()(pkt.sequence_nb) ^
                 (std::hash<int>()(pkt.start_sequence_nb) << 1)) >>
                1) ^
               (std::hash<int>()(pkt.end_sequence_nb) << 1);
    }
};
}  // namespace std

inline std::vector<char> serialize_packet(const packet& pkt) {
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

inline packet deserialize_packet(const std::vector<char>& buffer) {
    packet pkt;
    auto   it = buffer.begin();

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
    pkt.flag = static_cast<Flags>(flag);
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
        asio::ip::udp::endpoint(asio::ip::address::from_string(endpoint_address), endpoint_port);

    // Extract data
    pkt.data.assign(it, buffer.end());

    return pkt;
}

#endif  // PACKETUTILS_HPP