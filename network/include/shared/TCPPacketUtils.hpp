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

// #ifndef TCPPACKETUTILS_HPP
// #define TCPPACKETUTILS_HPP

// #include <algorithm>
// #include <vector>

// #include "PacketUtils.hpp"

// enum class TCPFlags {
//     CONTROL    = 0,
//     DATA       = 1,
//     HEARTBEAT  = 2,
//     NEW_CLIENT = 3,
//     TEST       = 4,
//     IMAGE      = 5,
// };

// struct TCPPacket {
//     int                     sequence_nb;
//     int                     start_sequence_nb;
//     int                     end_sequence_nb;
//     int                     packet_size;
//     TCPFlags                flag;
//     asio::ip::tcp::endpoint endpoint;
//     std::vector<char>       data;
//     std::string             file_name;

//     bool operator==(const TCPPacket& other) const {
//         return sequence_nb == other.sequence_nb && start_sequence_nb == other.start_sequence_nb
//         &&
//                end_sequence_nb == other.end_sequence_nb && packet_size == other.packet_size &&
//                flag == other.flag && data == other.data && endpoint == other.endpoint;
//     }
// };

// namespace std {
// template <>
// struct hash<TCPPacket> {
//     std::size_t operator()(const TCPPacket& pkt) const {
//         return ((std::hash<int>()(pkt.sequence_nb) ^
//                  (std::hash<int>()(pkt.start_sequence_nb) << 1)) >>
//                 1) ^
//                (std::hash<int>()(pkt.end_sequence_nb) << 1);
//     }
// };
// }  // namespace std

// inline std::vector<char> serialize(const TCPPacket& pkt) {
//     std::string endpoint_address = pkt.endpoint.address().to_string();
//     uint16_t    endpoint_port    = pkt.endpoint.port();

//     std::size_t total_size = sizeof(pkt.sequence_nb) + sizeof(pkt.start_sequence_nb) +
//                              sizeof(pkt.end_sequence_nb) + sizeof(pkt.packet_size) +
//                              sizeof(pkt.flag) + sizeof(uint16_t) + sizeof(std::size_t) +
//                              endpoint_address.size() + pkt.data.size();

//     std::vector<char> buffer(total_size);
//     auto              it = buffer.begin();

//     // Copy sequence_nb
//     std::copy(reinterpret_cast<const char*>(&pkt.sequence_nb),
//               reinterpret_cast<const char*>(&pkt.sequence_nb) + sizeof(pkt.sequence_nb), it);
//     it += sizeof(pkt.sequence_nb);

//     // Copy start_sequence_nb
//     std::copy(reinterpret_cast<const char*>(&pkt.start_sequence_nb),
//               reinterpret_cast<const char*>(&pkt.start_sequence_nb) +
//               sizeof(pkt.start_sequence_nb), it);
//     it += sizeof(pkt.start_sequence_nb);

//     // Copy end_sequence_nb
//     std::copy(reinterpret_cast<const char*>(&pkt.end_sequence_nb),
//               reinterpret_cast<const char*>(&pkt.end_sequence_nb) + sizeof(pkt.end_sequence_nb),
//               it);
//     it += sizeof(pkt.end_sequence_nb);

//     // Copy packet_size
//     std::copy(reinterpret_cast<const char*>(&pkt.packet_size),
//               reinterpret_cast<const char*>(&pkt.packet_size) + sizeof(pkt.packet_size), it);
//     it += sizeof(pkt.packet_size);

//     // Copy flag
//     int flag = static_cast<int>(pkt.flag);
//     std::copy(reinterpret_cast<const char*>(&flag),
//               reinterpret_cast<const char*>(&flag) + sizeof(flag), it);
//     it += sizeof(flag);

//     // Copy endpoint address length and address
//     std::size_t address_length = endpoint_address.size();
//     std::copy(reinterpret_cast<const char*>(&address_length),
//               reinterpret_cast<const char*>(&address_length) + sizeof(address_length), it);
//     it += sizeof(address_length);

//     std::copy(endpoint_address.begin(), endpoint_address.end(), it);
//     it += endpoint_address.size();

//     // Copy endpoint port
//     std::copy(reinterpret_cast<const char*>(&endpoint_port),
//               reinterpret_cast<const char*>(&endpoint_port) + sizeof(endpoint_port), it);
//     it += sizeof(endpoint_port);

//     // Copy data
//     std::copy(pkt.data.begin(), pkt.data.end(), it);

//     return buffer;
// }

// inline TCPPacket deserialize(const std::vector<char>& buffer) {
//     TCPPacket pkt;
//     auto      it = buffer.begin();

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
//     pkt.flag = static_cast<TCPFlags>(flag);
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
//         asio::ip::tcp::endpoint(asio::ip::address::from_string(endpoint_address), endpoint_port);

//     // Extract data
//     pkt.data.assign(it, buffer.end());

//     return pkt;
// }

// #endif  // PACKETUTILS_HPP

// // struct TCPPacket {
// //     int                     packet_size;
// //     TCPFlags                flag;
// //     std::vector<char>       data;
// //     asio::ip::tcp::endpoint endpoint;
// //     std::string             file_name;

// //     bool operator==(const TCPPacket& other) const {
// //         return packet_size == other.packet_size && flag == other.flag && data == other.data &&
// //                endpoint == other.endpoint && file_name == other.file_name;
// //     }

// //     // operator <<
// //     friend std::ostream& operator<<(std::ostream& os, const TCPPacket& packet) {
// //         os << "Packet Size: " << packet.packet_size << std::endl;
// //         os << "Flag: " << static_cast<int>(packet.flag) << std::endl;
// //         os << "Data: ";
// //         for (const auto& c : packet.data) {
// //             os << c;
// //         }
// //         os << std::endl;
// //         os << "Endpoint: " << packet.endpoint.address().to_string() << ":" <<
// //         packet.endpoint.port()
// //            << std::endl;
// //         if (packet.flag == TCPFlags::IMAGE) {
// //             os << "File Name: " << packet.file_name << std::endl;
// //         }
// //         return os;
// //     }

// //     // Serialize packet to binary
// //     std::vector<char> serialize() const {
// //         std::vector<char> buffer;

// //         // Serialize packet size
// //         buffer.insert(buffer.end(), reinterpret_cast<const char*>(&packet_size),
// //                       reinterpret_cast<const char*>(&packet_size) + sizeof(packet_size));

// //         // Serialize flag
// //         buffer.push_back(static_cast<char>(flag));

// //         // Serialize data size
// //         size_t data_size = data.size();
// //         buffer.insert(buffer.end(), reinterpret_cast<const char*>(&data_size),
// //                       reinterpret_cast<const char*>(&data_size) + sizeof(data_size));

// //         // Serialize data
// //         buffer.insert(buffer.end(), data.begin(), data.end());

// //         // Serialize endpoint (IP and port)
// //         auto   ip      = endpoint.address().to_string();
// //         size_t ip_size = ip.size();
// //         buffer.insert(buffer.end(), reinterpret_cast<const char*>(&ip_size),
// //                       reinterpret_cast<const char*>(&ip_size) + sizeof(ip_size));
// //         buffer.insert(buffer.end(), ip.begin(), ip.end());

// //         uint16_t port = endpoint.port();
// //         buffer.insert(buffer.end(), reinterpret_cast<const char*>(&port),
// //                       reinterpret_cast<const char*>(&port) + sizeof(port));

// //         // Serialize file name if flag is IMAGE
// //         if (flag == TCPFlags::IMAGE) {
// //             size_t file_name_size = file_name.size();
// //             buffer.insert(buffer.end(), reinterpret_cast<const char*>(&file_name_size),
// //                           reinterpret_cast<const char*>(&file_name_size) +
// //                           sizeof(file_name_size));
// //             buffer.insert(buffer.end(), file_name.begin(), file_name.end());
// //         }
// //         return buffer;
// //     }

// //     // Deserialize packet from binary
// //     static TCPPacket deserialize(const std::vector<char>& buffer) {
// //         TCPPacket packet;
// //         auto      it = buffer.begin();

// //         // Deserialize packet size
// //         std::memcpy(&packet.packet_size, &(*it), sizeof(packet.packet_size));
// //         it += sizeof(packet.packet_size);

// //         // Deserialize flag
// //         packet.flag = static_cast<TCPFlags>(*it++);

// //         // Deserialize data size
// //         size_t data_size;
// //         std::memcpy(&data_size, &(*it), sizeof(data_size));
// //         it += sizeof(data_size);
// //         std::cout << "Data size: " << data_size << std::endl;

// //         // Deserialize data
// //         packet.data = std::vector<char>(it, it + data_size);
// //         it += data_size;

// //         // Deserialize endpoint (IP and port)
// //         size_t ip_size;
// //         std::memcpy(&ip_size, &(*it), sizeof(ip_size));
// //         it += sizeof(ip_size);

// //         std::string ip(it, it + ip_size);
// //         it += ip_size;

// //         uint16_t port;
// //         std::memcpy(&port, &(*it), sizeof(port));
// //         it += sizeof(port);

// //         // Reconstruct endpoint
// //         packet.endpoint = asio::ip::tcp::endpoint(asio::ip::address::from_string(ip), port);

// //         // Deserialize file name if flag is IMAGE
// //         if (packet.flag == TCPFlags::IMAGE) {
// //             size_t file_name_size;
// //             std::memcpy(&file_name_size, &(*it), sizeof(file_name_size));
// //             it += sizeof(file_name_size);
// //             packet.file_name = std::string(it, it + file_name_size);
// //             it += file_name_size;
// //         }
// //         return packet;
// //     }
// // };

// // Structure for the TCP binary protocol
// // struct TCPPacket {
// //     TCPFlags                flag;
// //     std::vector<char>       data;
// //     asio::ip::tcp::endpoint endpoint;

// //     bool operator==(const TCPPacket& other) const {
// //         return flag == other.flag && data == other.data && endpoint == other.endpoint;
// //     }
// //     // operator <<
// //     friend std::ostream& operator<<(std::ostream& os, const TCPPacket& packet) {
// //         os << "Flag: " << static_cast<int>(packet.flag) << std::endl;
// //         os << "Data: ";
// //         for (const auto& c : packet.data) {
// //             os << c;
// //         }
// //         os << std::endl;
// //         os << "Endpoint: " << packet.endpoint.address().to_string() << ":" <<
// //         packet.endpoint.port()
// //            << std::endl;
// //         return os;
// //     }
// //     // Serialize packet to binary
// //     std::vector<char> serialize() const {
// //         std::vector<char> buffer;

// //         // Serialize flag
// //         buffer.push_back(static_cast<char>(flag));

// //         // Serialize data size
// //         size_t data_size = data.size();
// //         buffer.insert(buffer.end(), reinterpret_cast<const char*>(&data_size),
// //                       reinterpret_cast<const char*>(&data_size) + sizeof(data_size));

// //         // Serialize data
// //         buffer.insert(buffer.end(), data.begin(), data.end());

// //         // Serialize endpoint (IP and port)
// //         auto   ip      = endpoint.address().to_string();
// //         size_t ip_size = ip.size();
// //         buffer.insert(buffer.end(), reinterpret_cast<const char*>(&ip_size),
// //                       reinterpret_cast<const char*>(&ip_size) + sizeof(ip_size));
// //         buffer.insert(buffer.end(), ip.begin(), ip.end());

// //         uint16_t port = endpoint.port();
// //         buffer.insert(buffer.end(), reinterpret_cast<const char*>(&port),
// //                       reinterpret_cast<const char*>(&port) + sizeof(port));

// //         return buffer;
// //     }

// //     // Deserialize packet from binary
// //     static TCPPacket deserialize(const std::vector<char>& buffer) {
// //         TCPPacket packet;
// //         auto      it = buffer.begin();

// //         // Deserialize flag
// //         packet.flag = static_cast<TCPFlags>(*it++);

// //         // Deserialize data size
// //         size_t data_size;
// //         std::memcpy(&data_size, &(*it), sizeof(data_size));
// //         it += sizeof(data_size);

// //         // Deserialize data
// //         packet.data = std::vector<char>(it, it + data_size);
// //         it += data_size;

// //         // Deserialize endpoint (IP and port)
// //         size_t ip_size;
// //         std::memcpy(&ip_size, &(*it), sizeof(ip_size));
// //         it += sizeof(ip_size);

// //         std::string ip(it, it + ip_size);
// //         it += ip_size;

// //         uint16_t port;
// //         std::memcpy(&port, &(*it), sizeof(port));
// //         it += sizeof(port);

// //         // Reconstruct endpoint
// //         packet.endpoint = asio::ip::tcp::endpoint(asio::ip::address::from_string(ip), port);

// //         return packet;
// //     }
// // };
