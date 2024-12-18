/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** SendPackets.hpp
*/

#ifndef SENDPACKETS_HPP
#define SENDPACKETS_HPP

#include <array>
#include <asio.hpp>
#include <deque>
#include <iostream>
#include <map>
#include <queue>
#include <stack>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>

#include "PacketUtils.hpp"

// enum class Role { SERVER, CLIENT };

// Custom hash and equality for asio::ip::udp::endpoint
// we need it in order to know if we have already seen a client (kind of a select in C)

class PacketManager {
   public:
    PacketManager(asio::io_context& io_context, asio::ip::udp::socket& socket, Role role);
    ~PacketManager();

    void start();

    // packet functions
    // packet build_packet(int sequence_nb, int start_sequence_nb, int end_sequence_nb, Flags flag,
    //                     const asio::ip::udp::endpoint& endpoint, const std::string& message);
    packet build_packet(int sequence_nb, int start_sequence_nb, int end_sequence_nb, Flags flag,
                        const asio::ip::udp::endpoint&       endpoint,
                        const std::array<char, BUFFER_SIZE>& message);

    // threads function
    void receive();
    void send();
    void retry();

    // receive functions
    void handle_receive(std::size_t bytes_recvd);
    // handle messages
    void handle_ack(const std::string& ack_message);
    void handle_reliable_packet(const packet& pkt);
    void handle_unreliable_packet(const std::string& message);
    void handle_unreliable_packet(const std::array<char, BUFFER_SIZE>& message);
    void handle_new_client(const asio::ip::udp::endpoint& client_endpoint);
    void handle_test(const asio::ip::udp::endpoint& endpoint);

    // send functions
    void send_ack(SEQUENCE_TYPE sequence_start_number, SEQUENCE_TYPE sequence_number,
                  const asio::ip::udp::endpoint& endpoint_);
    void queue_packet_for_sending(const packet& pkt);
    void send_unreliable_packet(const std::string&             message,
                                const asio::ip::udp::endpoint& endpoint);
    void send_packet(const packet& pkt);
    void send_reliable_packet(const std::string& message, const asio::ip::udp::endpoint& endpoint);
    void send_new_client(const asio::ip::udp::endpoint& endpoint);
    void send_test(const asio::ip::udp::endpoint& endpoint);

    // vector char overload
    void send_reliable_packet(const std::array<char, BUFFER_SIZE>& message,
                              const asio::ip::udp::endpoint&       endpoint);
    void send_unreliable_packet(const std::vector<char>&       message,
                                const asio::ip::udp::endpoint& endpoint);

    void send_unreliable_packet(const std::array<char, BUFFER_SIZE>& message,
                                const asio::ip::udp::endpoint&       endpoint);

    // retry functions
    void queue_packet_for_retry(const packet& pkt);

    // void schedule_retransmissions(const asio::ip::udp::endpoint& endpoint);
    // std::queue<packet> get_received_packets();
    std::unordered_set<asio::ip::udp::endpoint, EndpointHash, EndpointEqual> getKnownClients();

    // overload for vector<char>
    const std::array<char, BUFFER_SIZE> get_last_reliable_packet_data();
    const std::array<char, BUFFER_SIZE> get_last_unreliable_packet_data();

    // std::vector<std::string> get_unreliable_messages_from_endpoint(
    //     const asio::ip::udp::endpoint& endpoint);
    // std::vector<std::string> get_reliable_messages_from_endpoint(
    //     const asio::ip::udp::endpoint& endpoint);

    // overload for vector<char>
    // std::vector<std::vector<char>> get_unreliable_messages_from_endpoint_data(
    //     const asio::ip::udp::endpoint& endpoint);
    // std::vector<std::vector<char>> get_reliable_messages_from_endpoint_data(
    //     const asio::ip::udp::endpoint& endpoint);

    std::vector<std::array<char, BUFFER_SIZE>> get_unreliable_messages_from_endpoint_data(
        const asio::ip::udp::endpoint& endpoint);
    std::vector<std::array<char, BUFFER_SIZE>> get_reliable_messages_from_endpoint_data(
        const asio::ip::udp::endpoint& endpoint);

    // void               retransmit_unacknowledged_packets(const asio::ip::udp::endpoint&
    // endpoint);
    void print_packet(const packet& pkt);

    std::function<void(const asio::ip::udp::endpoint& client_endpoint)> _new_client_callback;

   private:
    asio::steady_timer     retransmission_timer_;
    asio::ip::udp::socket& socket_;
    Role                   role_;

    std::unordered_set<asio::ip::udp::endpoint, EndpointHash, EndpointEqual> _known_clients;

    bool                   _stop_processing;
    std::array<char, 1024> recv_buffer_;

    asio::ip::udp::endpoint _endpoint;  // the endpoint that sent the last message

    // threads
    std::thread _send_packet_thread;
    std::thread _receive_packet_thread;
    std::thread _retry_packet_thread;

    // receive packets variables
    SEQUENCE_TYPE _message_id;  // start at 0 and increment of (packet number) for each message
    std::mutex    _message_id_mutex;

    std::unordered_map<int, std::vector<packet>> _received_packets;
    std::mutex                                   _received_packets_mutex;

    // send packets variables
    std::deque<packet>         _send_queue;
    std::mutex                 _send_queue_mutex;
    std::condition_variable    _send_queue_cv;
    std::unordered_set<packet> _send_queue_set;

    // retry packets variables
    std::deque<packet> _retry_queue;
    std::mutex         _retry_queue_mutex;

    // // std::stack<std::string> _unprocessed_unreliable_messages;
    // std::vector<std::pair<std::string, asio::ip::udp::endpoint>>
    // _unprocessed_unreliable_messages; std::mutex _unprocessed_unreliable_messages_mutex;

    std::vector<std::pair<std::array<char, BUFFER_SIZE>, asio::ip::udp::endpoint>>
               _unprocessed_unreliable_messages_data;
    std::mutex _unprocessed_unreliable_messages_data_mutex;

    // // std::stack<std::string> _unprocessed_reliable_messages;
    // std::vector<std::pair<std::string, asio::ip::udp::endpoint>> _unprocessed_reliable_messages;
    // std::mutex _unprocessed_reliable_messages_mutex;

    std::vector<std::pair<std::array<char, BUFFER_SIZE>, asio::ip::udp::endpoint>>
               _unprocessed_reliable_messages_data;
    std::mutex _unprocessed_reliable_messages_data_mutex;

    // std::mutex  _message_complete_mutex;
    // std::string _message_complete_buffer;

    // work guard
    asio::executor_work_guard<asio::io_context::executor_type> work_guard_;
};

#endif  // SENDPACKETS_HPP