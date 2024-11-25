#ifndef PACKETUTILS_HPP
#define PACKETUTILS_HPP

#include <vector>
#include <algorithm>

#define BUFFER_SIZE 500
#define WINDOW_SIZE 5

enum Flags { UNRELIABLE = 0, RELIABLE = 1, ACK = 2, CONTROL = 3, HEARTBEAT = 4, DATA = 5 };

// structure for the binary protocol
struct packet {
    int               sequence_no;
    int               start_sequence_no;
    int               end_sequence_no;
    int               packet_size;
    Flags             flag;
    std::vector<char> data;
};

// inline improves performance by avoiding the overhead of function calls
inline std::vector<char> serialize_packet(const packet& pkt) {
    std::size_t total_size = sizeof(pkt.sequence_no) + sizeof(pkt.start_sequence_no) +
                             sizeof(pkt.end_sequence_no) + sizeof(pkt.packet_size) +
                             sizeof(pkt.flag) + pkt.data.size();
    std::vector<char> buffer(total_size);
    auto              it = buffer.begin();

    // Copy sequence_no to the buffer
    std::copy(reinterpret_cast<const char*>(&pkt.sequence_no),
              reinterpret_cast<const char*>(&pkt.sequence_no) + sizeof(pkt.sequence_no), it);
    it += sizeof(pkt.sequence_no);

    // Copy start_sequence_no to the buffer
    std::copy(reinterpret_cast<const char*>(&pkt.start_sequence_no),
              reinterpret_cast<const char*>(&pkt.start_sequence_no) + sizeof(pkt.start_sequence_no),
              it);
    it += sizeof(pkt.start_sequence_no);

    // Copy max_sequence_no to the buffer
    std::copy(reinterpret_cast<const char*>(&pkt.end_sequence_no),
              reinterpret_cast<const char*>(&pkt.end_sequence_no) + sizeof(pkt.end_sequence_no),
              it);
    it += sizeof(pkt.end_sequence_no);

    // Copy packet_size to the buffer
    std::copy(reinterpret_cast<const char*>(&pkt.packet_size),
              reinterpret_cast<const char*>(&pkt.packet_size) + sizeof(pkt.packet_size), it);
    it += sizeof(pkt.packet_size);

    // Copy flag to the buffer
    int flag = static_cast<int>(pkt.flag);
    std::copy(reinterpret_cast<const char*>(&flag),
              reinterpret_cast<const char*>(&flag) + sizeof(flag), it);
    it += sizeof(pkt.flag);

    // Copy data to the buffer
    std::copy(pkt.data.begin(), pkt.data.end(), it);

    return buffer;
}

// inline improves performance by avoiding the overhead of function calls
inline packet deserialize_packet(const std::vector<char>& buffer) {
    packet pkt;
    auto   it = buffer.begin();

    // Extract sequence_no from the buffer
    std::copy(it, it + sizeof(pkt.sequence_no), reinterpret_cast<char*>(&pkt.sequence_no));
    it += sizeof(pkt.sequence_no);

    // Extract start_sequence_no from the buffer
    std::copy(it, it + sizeof(pkt.start_sequence_no),
              reinterpret_cast<char*>(&pkt.start_sequence_no));
    it += sizeof(pkt.start_sequence_no);

    // Extract max_sequence_no from the buffer
    std::copy(it, it + sizeof(pkt.end_sequence_no), reinterpret_cast<char*>(&pkt.end_sequence_no));
    it += sizeof(pkt.end_sequence_no);

    // Extract packet_size from the buffer
    std::copy(it, it + sizeof(pkt.packet_size), reinterpret_cast<char*>(&pkt.packet_size));
    it += sizeof(pkt.packet_size);

    // Extract flag from the buffer
    int flag;
    std::copy(it, it + sizeof(flag), reinterpret_cast<char*>(&flag));
    pkt.flag = static_cast<Flags>(flag);
    it += sizeof(flag);

    // Extract data from the buffer
    pkt.data.assign(it, buffer.end());

    return pkt;
}

#endif // PACKETUTILS_HPP