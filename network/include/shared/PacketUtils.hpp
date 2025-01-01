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
// #define BUFFER_SIZE 800

enum class Role { SERVER, CLIENT };

// Custom hash and equality for asio::ip::udp::endpoint
// we need it in order to know if we have already seen a client (kind of a select in C)
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
    UNRELIABLE    = 1,
    RELIABLE      = 2,
    ACK           = 3,
    CONTROL       = 4,
    HEARTBEAT     = 5,
    DATA          = 6,
    RETRANSMITTED = 7,
    NEW_CLIENT    = 8,
    TEST          = 9,
};

enum class AckType { CLIENT_ACK, ACK };

struct AckMessage {
    AckType  ack_type;
    uint64_t start_sequence_number;
    uint64_t sequence_number;
};

template <std::size_t BUFFER_SIZE>
void serialize_ack(const AckMessage& ack, std::array<char, BUFFER_SIZE>& buffer) {
    // Ensure the buffer is large enough to hold the serialized data
    static_assert(
        sizeof(ack.ack_type) + sizeof(ack.start_sequence_number) + sizeof(ack.sequence_number) <=
            BUFFER_SIZE,
        "Buffer size is too small for AckMessage serialization");

    // Copy the AckType enum value to the buffer
    buffer[0] = static_cast<char>(ack.ack_type);  // Save AckType as a byte

    // Copy the sequence numbers to the buffer (in a platform-independent way)
    std::memcpy(buffer.data() + 1, &ack.start_sequence_number, sizeof(ack.start_sequence_number));
    std::memcpy(buffer.data() + 1 + sizeof(ack.start_sequence_number), &ack.sequence_number,
                sizeof(ack.sequence_number));
}

template <std::size_t BUFFER_SIZE>
AckMessage deserialize_ack(const std::array<char, BUFFER_SIZE>& buffer) {
    AckMessage ackMessage;

    // Deserialize the AckType (first byte)
    ackMessage.ack_type = static_cast<AckType>(buffer[0]);

    // Deserialize the sequence numbers
    std::memcpy(&ackMessage.start_sequence_number, buffer.data() + 1,
                sizeof(ackMessage.start_sequence_number));
    std::memcpy(&ackMessage.sequence_number,
                buffer.data() + 1 + sizeof(ackMessage.start_sequence_number),
                sizeof(ackMessage.sequence_number));

    return ackMessage;
}

template <std::size_t BUFFER_SIZE>
struct packet {
    SEQUENCE_TYPE                 sequence_nb;
    SEQUENCE_TYPE                 start_sequence_nb;
    SEQUENCE_TYPE                 end_sequence_nb;
    SEQUENCE_TYPE                 packet_size;
    Flags                         flag;
    asio::ip::udp::endpoint       endpoint;
    std::array<char, BUFFER_SIZE> data;

    bool operator==(const packet& other) const {
        return sequence_nb == other.sequence_nb && start_sequence_nb == other.start_sequence_nb &&
               end_sequence_nb == other.end_sequence_nb && packet_size == other.packet_size &&
               flag == other.flag && data == other.data && endpoint == other.endpoint;
    }
};

namespace std {
template <std::size_t BUFFER_SIZE>
struct hash<packet<BUFFER_SIZE>> {
    std::size_t operator()(const packet<BUFFER_SIZE>& pkt) const {
        return ((std::hash<int>()(pkt.sequence_nb) ^
                 (std::hash<int>()(pkt.start_sequence_nb) << 1)) >>
                1) ^
               (std::hash<int>()(pkt.end_sequence_nb) << 1);
    }
};
}  // namespace std

template <std::size_t BUFFER_SIZE>
inline std::vector<char> serialize_packet(const packet<BUFFER_SIZE>& pkt) {
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

// inline packet deserialize_packet(const std::vector<char>& buffer) {
//     packet pkt;
//     auto   it = buffer.begin();

//     // Extract sequence_nb
//     std::copy(it, it + sizeof(pkt.sequence_nb), reinterpret_cast<char*>(&pkt.sequence_nb));
//     it += sizeof(pkt.sequence_nb);

//     // Extract start_sequence_nb
//     std::copy(it, it + sizeof(pkt.start_sequence_nb),
//               reinterpret_cast<char*>(&pkt.start_sequence_nb));
//     it += sizeof(pkt.start_sequence_nb);

//     // Extract end_sequence_nb
//     std::copy(it, it + sizeof(pkt.end_sequence_nb),
//     reinterpret_cast<char*>(&pkt.end_sequence_nb)); it += sizeof(pkt.end_sequence_nb);

//     // Extract packet_size
//     std::copy(it, it + sizeof(pkt.packet_size), reinterpret_cast<char*>(&pkt.packet_size));
//     it += sizeof(pkt.packet_size);

//     // Extract flag
//     int flag;
//     std::copy(it, it + sizeof(flag), reinterpret_cast<char*>(&flag));
//     pkt.flag = static_cast<Flags>(flag);
//     it += sizeof(flag);

//     // Extract endpoint address length and address
//     std::size_t address_length;
//     std::copy(it, it + sizeof(address_length), reinterpret_cast<char*>(&address_length));
//     it += sizeof(address_length);

//     std::string endpoint_address(it, it + address_length);
//     it += address_length;

//     // Extract endpoint port
//     uint16_t endpoint_port;
//     std::copy(it, it + sizeof(endpoint_port), reinterpret_cast<char*>(&endpoint_port));
//     it += sizeof(endpoint_port);

//     pkt.endpoint =
//         asio::ip::udp::endpoint(asio::ip::address::from_string(endpoint_address), endpoint_port);

//     // Extract data
//     pkt.data.assign(it, buffer.end());

//     return pkt;
// }

template <std::size_t BUFFER_SIZE>
inline packet<BUFFER_SIZE> deserialize_packet(const std::array<char, BUFFER_SIZE>& buffer) {
    packet<BUFFER_SIZE> pkt;
    auto                it = buffer.begin();

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
    std::copy(it, buffer.end(), pkt.data.begin());

    return pkt;
}

#endif  // PACKETUTILS_HPP