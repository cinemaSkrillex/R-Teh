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

enum class TCPFlags {
    CONTROL    = 0,
    DATA       = 1,
    HEARTBEAT  = 2,
    NEW_CLIENT = 3,
    TEST       = 4,
};

struct TCPPacket {
    int                     packet_size;
    TCPFlags                flag;
    std::vector<char>       data;
    asio::ip::tcp::endpoint endpoint;

    bool operator==(const TCPPacket& other) const {
        return packet_size == other.packet_size && flag == other.flag && data == other.data &&
               endpoint == other.endpoint;
    }

    // operator <<
    friend std::ostream& operator<<(std::ostream& os, const TCPPacket& packet) {
        os << "Packet Size: " << packet.packet_size << std::endl;
        os << "Flag: " << static_cast<int>(packet.flag) << std::endl;
        os << "Data: ";
        for (const auto& c : packet.data) {
            os << c;
        }
        os << std::endl;
        os << "Endpoint: " << packet.endpoint.address().to_string() << ":" << packet.endpoint.port()
           << std::endl;
        return os;
    }

    // Serialize packet to binary
    std::vector<char> serialize() const {
        std::vector<char> buffer;

        // Serialize packet size
        buffer.insert(buffer.end(), reinterpret_cast<const char*>(&packet_size),
                      reinterpret_cast<const char*>(&packet_size) + sizeof(packet_size));

        // Serialize flag
        buffer.push_back(static_cast<char>(flag));

        // Serialize data size
        size_t data_size = data.size();
        buffer.insert(buffer.end(), reinterpret_cast<const char*>(&data_size),
                      reinterpret_cast<const char*>(&data_size) + sizeof(data_size));

        // Serialize data
        buffer.insert(buffer.end(), data.begin(), data.end());

        // Serialize endpoint (IP and port)
        auto   ip      = endpoint.address().to_string();
        size_t ip_size = ip.size();
        buffer.insert(buffer.end(), reinterpret_cast<const char*>(&ip_size),
                      reinterpret_cast<const char*>(&ip_size) + sizeof(ip_size));
        buffer.insert(buffer.end(), ip.begin(), ip.end());

        uint16_t port = endpoint.port();
        buffer.insert(buffer.end(), reinterpret_cast<const char*>(&port),
                      reinterpret_cast<const char*>(&port) + sizeof(port));

        return buffer;
    }

    // Deserialize packet from binary
    static TCPPacket deserialize(const std::vector<char>& buffer) {
        TCPPacket packet;
        auto      it = buffer.begin();

        // Deserialize packet size
        std::memcpy(&packet.packet_size, &(*it), sizeof(packet.packet_size));
        it += sizeof(packet.packet_size);

        // Deserialize flag
        packet.flag = static_cast<TCPFlags>(*it++);

        // Deserialize data size
        size_t data_size;
        std::memcpy(&data_size, &(*it), sizeof(data_size));
        it += sizeof(data_size);

        // Deserialize data
        packet.data = std::vector<char>(it, it + data_size);
        it += data_size;

        // Deserialize endpoint (IP and port)
        size_t ip_size;
        std::memcpy(&ip_size, &(*it), sizeof(ip_size));
        it += sizeof(ip_size);

        std::string ip(it, it + ip_size);
        it += ip_size;

        uint16_t port;
        std::memcpy(&port, &(*it), sizeof(port));
        it += sizeof(port);

        // Reconstruct endpoint
        packet.endpoint = asio::ip::tcp::endpoint(asio::ip::address::from_string(ip), port);

        return packet;
    }
};

// Structure for the TCP binary protocol
// struct TCPPacket {
//     TCPFlags                flag;
//     std::vector<char>       data;
//     asio::ip::tcp::endpoint endpoint;

//     bool operator==(const TCPPacket& other) const {
//         return flag == other.flag && data == other.data && endpoint == other.endpoint;
//     }
//     // operator <<
//     friend std::ostream& operator<<(std::ostream& os, const TCPPacket& packet) {
//         os << "Flag: " << static_cast<int>(packet.flag) << std::endl;
//         os << "Data: ";
//         for (const auto& c : packet.data) {
//             os << c;
//         }
//         os << std::endl;
//         os << "Endpoint: " << packet.endpoint.address().to_string() << ":" <<
//         packet.endpoint.port()
//            << std::endl;
//         return os;
//     }
//     // Serialize packet to binary
//     std::vector<char> serialize() const {
//         std::vector<char> buffer;

//         // Serialize flag
//         buffer.push_back(static_cast<char>(flag));

//         // Serialize data size
//         size_t data_size = data.size();
//         buffer.insert(buffer.end(), reinterpret_cast<const char*>(&data_size),
//                       reinterpret_cast<const char*>(&data_size) + sizeof(data_size));

//         // Serialize data
//         buffer.insert(buffer.end(), data.begin(), data.end());

//         // Serialize endpoint (IP and port)
//         auto   ip      = endpoint.address().to_string();
//         size_t ip_size = ip.size();
//         buffer.insert(buffer.end(), reinterpret_cast<const char*>(&ip_size),
//                       reinterpret_cast<const char*>(&ip_size) + sizeof(ip_size));
//         buffer.insert(buffer.end(), ip.begin(), ip.end());

//         uint16_t port = endpoint.port();
//         buffer.insert(buffer.end(), reinterpret_cast<const char*>(&port),
//                       reinterpret_cast<const char*>(&port) + sizeof(port));

//         return buffer;
//     }

//     // Deserialize packet from binary
//     static TCPPacket deserialize(const std::vector<char>& buffer) {
//         TCPPacket packet;
//         auto      it = buffer.begin();

//         // Deserialize flag
//         packet.flag = static_cast<TCPFlags>(*it++);

//         // Deserialize data size
//         size_t data_size;
//         std::memcpy(&data_size, &(*it), sizeof(data_size));
//         it += sizeof(data_size);

//         // Deserialize data
//         packet.data = std::vector<char>(it, it + data_size);
//         it += data_size;

//         // Deserialize endpoint (IP and port)
//         size_t ip_size;
//         std::memcpy(&ip_size, &(*it), sizeof(ip_size));
//         it += sizeof(ip_size);

//         std::string ip(it, it + ip_size);
//         it += ip_size;

//         uint16_t port;
//         std::memcpy(&port, &(*it), sizeof(port));
//         it += sizeof(port);

//         // Reconstruct endpoint
//         packet.endpoint = asio::ip::tcp::endpoint(asio::ip::address::from_string(ip), port);

//         return packet;
//     }
// };

#endif  // TCPPACKETUTILS_HPP
