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

#define SEQUENCE_TYPE std::uint32_t

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

template <std::size_t BUFFER_SIZE>
struct packet {
    SEQUENCE_TYPE                 sequenceNb;
    SEQUENCE_TYPE                 startSequenceNb;
    SEQUENCE_TYPE                 endSequenceNb;
    SEQUENCE_TYPE                 packetSize;
    Flags                         flag;
    asio::ip::udp::endpoint       endpoint;
    std::array<char, BUFFER_SIZE> data;

    bool operator==(const packet& other) const {
        return sequenceNb == other.sequenceNb && startSequenceNb == other.startSequenceNb &&
               endSequenceNb == other.endSequenceNb && packetSize == other.packetSize &&
               flag == other.flag && data == other.data && endpoint == other.endpoint;
    }
};

namespace std {
template <std::size_t BUFFER_SIZE>
struct hash<packet<BUFFER_SIZE>> {
    std::size_t operator()(const packet<BUFFER_SIZE>& pkt) const {
        return ((std::hash<int>()(pkt.sequenceNb) ^ (std::hash<int>()(pkt.startSequenceNb) << 1)) >>
                1) ^
               (std::hash<int>()(pkt.endSequenceNb) << 1);
    }
};
}  // namespace std

template <std::size_t BUFFER_SIZE>
inline std::vector<char> serialize_packet(const packet<BUFFER_SIZE>& pkt) {
    std::string endpointAddress = pkt.endpoint.address().to_string();
    uint16_t    endpointPort    = pkt.endpoint.port();

    std::size_t totalSize = sizeof(pkt.sequenceNb) + sizeof(pkt.startSequenceNb) +
                            sizeof(pkt.endSequenceNb) + sizeof(pkt.packetSize) + sizeof(pkt.flag) +
                            sizeof(uint16_t) + sizeof(std::size_t) + endpointAddress.size() +
                            pkt.data.size();

    std::vector<char> buffer(totalSize);
    auto              it = buffer.begin();

    // Copy sequenceNb
    std::copy(reinterpret_cast<const char*>(&pkt.sequenceNb),
              reinterpret_cast<const char*>(&pkt.sequenceNb) + sizeof(pkt.sequenceNb), it);
    it += sizeof(pkt.sequenceNb);

    // Copy startSequenceNb
    std::copy(reinterpret_cast<const char*>(&pkt.startSequenceNb),
              reinterpret_cast<const char*>(&pkt.startSequenceNb) + sizeof(pkt.startSequenceNb),
              it);
    it += sizeof(pkt.startSequenceNb);

    // Copy endSequenceNb
    std::copy(reinterpret_cast<const char*>(&pkt.endSequenceNb),
              reinterpret_cast<const char*>(&pkt.endSequenceNb) + sizeof(pkt.endSequenceNb), it);
    it += sizeof(pkt.endSequenceNb);

    // Copy packetSize
    std::copy(reinterpret_cast<const char*>(&pkt.packetSize),
              reinterpret_cast<const char*>(&pkt.packetSize) + sizeof(pkt.packetSize), it);
    it += sizeof(pkt.packetSize);

    // Copy flag
    int flag = static_cast<int>(pkt.flag);
    std::copy(reinterpret_cast<const char*>(&flag),
              reinterpret_cast<const char*>(&flag) + sizeof(flag), it);
    it += sizeof(flag);

    // Copy endpoint address length and address
    std::size_t addressLength = endpointAddress.size();
    std::copy(reinterpret_cast<const char*>(&addressLength),
              reinterpret_cast<const char*>(&addressLength) + sizeof(addressLength), it);
    it += sizeof(addressLength);

    std::copy(endpointAddress.begin(), endpointAddress.end(), it);
    it += endpointAddress.size();

    // Copy endpoint port
    std::copy(reinterpret_cast<const char*>(&endpointPort),
              reinterpret_cast<const char*>(&endpointPort) + sizeof(endpointPort), it);
    it += sizeof(endpointPort);

    // Copy data
    std::copy(pkt.data.begin(), pkt.data.end(), it);

    return buffer;
}

template <std::size_t BUFFER_SIZE>
inline packet<BUFFER_SIZE> deserializePacket(const std::array<char, BUFFER_SIZE>& buffer) {
    packet<BUFFER_SIZE> pkt;
    auto                it = buffer.begin();

    // Extract sequenceNb
    std::copy(it, it + sizeof(pkt.sequenceNb), reinterpret_cast<char*>(&pkt.sequenceNb));
    it += sizeof(pkt.sequenceNb);

    // Extract startSequenceNb
    std::copy(it, it + sizeof(pkt.startSequenceNb), reinterpret_cast<char*>(&pkt.startSequenceNb));
    it += sizeof(pkt.startSequenceNb);

    // Extract endSequenceNb
    std::copy(it, it + sizeof(pkt.endSequenceNb), reinterpret_cast<char*>(&pkt.endSequenceNb));
    it += sizeof(pkt.endSequenceNb);

    // Extract packetSize
    std::copy(it, it + sizeof(pkt.packetSize), reinterpret_cast<char*>(&pkt.packetSize));
    it += sizeof(pkt.packetSize);

    // Extract flag
    int flag;
    std::copy(it, it + sizeof(flag), reinterpret_cast<char*>(&flag));
    pkt.flag = static_cast<Flags>(flag);
    it += sizeof(flag);

    // Extract endpoint address length and address
    std::size_t addressLength;
    std::copy(it, it + sizeof(addressLength), reinterpret_cast<char*>(&addressLength));
    it += sizeof(addressLength);

    std::string endpointAddress(it, it + addressLength);
    it += addressLength;

    // Extract endpoint port
    uint16_t endpointPort;
    std::copy(it, it + sizeof(endpointPort), reinterpret_cast<char*>(&endpointPort));
    it += sizeof(endpointPort);

    pkt.endpoint =
        asio::ip::udp::endpoint(asio::ip::address::from_string(endpointAddress), endpointPort);

    // Extract data
    std::copy(it, buffer.end(), pkt.data.begin());

    return pkt;
}

// Ack packet

enum class AckType { CLIENT_ACK, ACK };

struct AckMessage {
    AckType  ack_type;
    uint64_t startSequenceNumber;
    uint64_t sequenceNumber;
};

template <std::size_t BUFFER_SIZE>
void serialize_ack(const AckMessage& ack, std::array<char, BUFFER_SIZE>& buffer) {
    // Ensure the buffer is large enough to hold the serialized data
    static_assert(
        sizeof(ack.ack_type) + sizeof(ack.startSequenceNumber) + sizeof(ack.sequenceNumber) <=
            BUFFER_SIZE,
        "Buffer size is too small for AckMessage serialization");

    // Copy the AckType enum value to the buffer
    buffer[0] = static_cast<char>(ack.ack_type);

    // Copy the sequence numbers to the buffer (in a platform-independent way)
    std::memcpy(buffer.data() + 1, &ack.startSequenceNumber, sizeof(ack.startSequenceNumber));
    std::memcpy(buffer.data() + 1 + sizeof(ack.startSequenceNumber), &ack.sequenceNumber,
                sizeof(ack.sequenceNumber));
}

template <std::size_t BUFFER_SIZE>
AckMessage deserializeAck(const std::array<char, BUFFER_SIZE>& buffer) {
    AckMessage ackMessage;

    // Deserialize the AckType (first byte)
    ackMessage.ack_type = static_cast<AckType>(buffer[0]);

    // Deserialize the sequence numbers
    std::memcpy(&ackMessage.startSequenceNumber, buffer.data() + 1,
                sizeof(ackMessage.startSequenceNumber));
    std::memcpy(&ackMessage.sequenceNumber,
                buffer.data() + 1 + sizeof(ackMessage.startSequenceNumber),
                sizeof(ackMessage.sequenceNumber));

    return ackMessage;
}

#endif  // PACKETUTILS_HPP