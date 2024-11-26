/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** SendPackets.hpp
*/

#ifndef SENDPACKETS_HPP
#define SENDPACKETS_HPP

#include <asio.hpp>
#include <iostream>
#include <array>
#include <string>
#include <map>
#include <thread>
#include <queue>
#include <stack>
#include <unordered_map>
#include "PacketUtils.hpp"
#include "ThreadPool.hpp"
#include "PacketQueueManager.hpp"

#include <unordered_set>

// Custom hash and equality for asio::ip::udp::endpoint
// we need it in order to know if we have already seen a client (kind of a select in C)
struct EndpointHash {
    std::size_t operator()(const asio::ip::udp::endpoint& endpoint) const {
        std::size_t h1 = std::hash<std::string>()(endpoint.address().to_string());
        std::size_t h2 = std::hash<unsigned short>()(endpoint.port());
        return h1 ^ (h2 << 1); // Combine the hashes
    }
};

struct EndpointEqual {
    bool operator()(const asio::ip::udp::endpoint& lhs, const asio::ip::udp::endpoint& rhs) const {
        return lhs.address() == rhs.address() && lhs.port() == rhs.port();
    }
};

enum class Role { SERVER, CLIENT };

// WARNING: can reduce performance because ONLY HEADER
class PacketManager {
  public:
    PacketManager(asio::io_context& io_context, asio::ip::udp::socket& socket, Role role);
    void start();
    ~PacketManager();

    void start_receive();
    void handle_receive(std::size_t bytes_recvd);

    void process_packets();

    void handle_new_client(const asio::ip::udp::endpoint& endpoint);
    void handle_reliable_packet(const packet& pkt);
    void handle_unreliable_packet(const std::string& message);
    void send_packets();

    void handle_retransmissions();

    void queue_packet_for_retransmission(const packet&                  pkt,
                                         const asio::ip::udp::endpoint& endpoint);

    void schedule_retransmissions(const asio::ip::udp::endpoint& endpoint);

    void handle_ack(const std::string& ack_message);

    void send_reliable_packet(const std::string& message, const asio::ip::udp::endpoint& endpoint);
    void queue_packet_for_sending(const packet& pkt, const asio::ip::udp::endpoint& endpoint);
    void send_unreliable_packet(const std::string&             message,
                                const asio::ip::udp::endpoint& endpoint);

    void send_ack(std::uint32_t sequence_number, const asio::ip::udp::endpoint& endpoint_);
    std::queue<packet> get_received_packets();

    std::string get_last_unreliable_packet() {
        std::lock_guard<std::mutex> lock(_unprocessed_unreliable_messages_mutex);
        std::string                 message = _unprocessed_unreliable_messages.top();
        _unprocessed_unreliable_messages.pop();
        return message;
    }

  private:
    void send_packet(const packet& pkt, const asio::ip::udp::endpoint& endpoint);

    // TODO change to a MACRO for changing the sequence number max allocated size
    std::uint32_t                             sequence_number_;
    std::unordered_map<std::uint32_t, packet> unacknowledged_packets_;
    std::mutex                                unacknowledged_packets_mutex_;
    asio::steady_timer                        retransmission_timer_;
    asio::ip::udp::socket&                    socket_;
    Role                                      role_;

    std::unordered_set<asio::ip::udp::endpoint, EndpointHash, EndpointEqual> known_clients_;

    std::queue<packet>      packet_queue_;
    std::mutex              queue_mutex_;
    std::condition_variable queue_cv_;
    std::thread             receive_packet_thread_;
    std::thread             process_packet_thread_;
    std::thread             send_packet_thread_;
    bool                    stop_processing_;
    std::array<char, 1024>  recv_buffer_;
    asio::ip::udp::endpoint remote_endpoint_;

    // Store received packets
    std::map<int, packet> received_packets_;
    int                   start_sequence_no_ = -1;
    int                   end_sequence_no    = -1;

    ThreadPool thread_pool_;

    // std::queue<std::pair<packet, asio::ip::udp::endpoint>> send_queue_;
    std::mutex send_queue_mutex_;
    // std::condition_variable                                send_queue_cv_;

    // std::queue<std::pair<packet, asio::ip::udp::endpoint>> retransmission_queue_;
    // std::mutex                                             retransmission_queue_mutex_;
    // std::condition_variable                                retransmission_queue_cv_;
    // std::thread                                            retransmission_thread_;
    PacketQueueManager send_queue_;
    PacketQueueManager retransmission_queue_;

    std::stack<std::string> _unprocessed_unreliable_messages;
    std::mutex              _unprocessed_unreliable_messages_mutex;

    // work guard
    asio::executor_work_guard<asio::io_context::executor_type> work_guard_;
};

#endif // SENDPACKETS_HPP