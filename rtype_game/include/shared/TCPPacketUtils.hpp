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
    int                     sequenceNb;
    int                     startSequenceNb;
    int                     endSequenceNb;
    int                     packetSize;
    TCPFlags                flag;
    asio::ip::tcp::endpoint endpoint;
    std::vector<char>       data;

    bool operator==(const TCPPacket& other) const {
        return sequenceNb == other.sequenceNb && startSequenceNb == other.startSequenceNb &&
               endSequenceNb == other.endSequenceNb && packetSize == other.packetSize &&
               flag == other.flag && data == other.data && endpoint == other.endpoint;
    }
};

namespace std {
template <>
struct hash<TCPPacket> {
    std::size_t operator()(const TCPPacket& pkt) const {
        return ((std::hash<int>()(pkt.sequenceNb) ^ (std::hash<int>()(pkt.startSequenceNb) << 1)) >>
                1) ^
               (std::hash<int>()(pkt.endSequenceNb) << 1);
    }
};
}  // namespace std

inline std::vector<char> serialize(const TCPPacket& pkt) {
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

inline TCPPacket deserialize(const std::vector<char>& buffer) {
    TCPPacket pkt;
    auto      it = buffer.begin();

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
    pkt.flag = static_cast<TCPFlags>(flag);
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
        asio::ip::tcp::endpoint(asio::ip::address::from_string(endpointAddress), endpointPort);

    // Extract data
    pkt.data.assign(it, buffer.end());

    return pkt;
}