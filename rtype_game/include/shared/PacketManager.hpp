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

// Templates need to have their implementation in the header file see the following link for more
// https://stackoverflow.com/a/1353981

// enum class Role { SERVER, CLIENT };

// Custom hash and equality for asio::ip::udp::endpoint
// we need it in order to know if we have already seen a client (kind of a select in C)

template <std::size_t BUFFER_SIZE>
class PacketManager {
   public:
    PacketManager(asio::io_context& io_context, asio::ip::udp::socket& socket, Role role)
        : _message_id(0),
          retransmission_timer_(io_context),
          socket_(socket),
          role_(role),
          _stop_processing(false),
          work_guard_(asio::make_work_guard(io_context)) {}
    ~PacketManager() {
        _stop_processing = true;
        if (_send_packet_thread.joinable()) _send_packet_thread.join();
        if (_receive_packet_thread.joinable()) _receive_packet_thread.join();
        if (_retry_packet_thread.joinable()) _retry_packet_thread.join();
    }

    void start() {
        _receive_packet_thread = std::thread(&PacketManager::receive, this);
        _send_packet_thread    = std::thread(&PacketManager::send, this);
        _retry_packet_thread   = std::thread(&PacketManager::retry, this);
    }

    // packet functions
    // packet build_packet(int sequence_nb, int start_sequence_nb, int end_sequence_nb, Flags flag,
    //                     const asio::ip::udp::endpoint& endpoint, const std::string& message);
    packet<BUFFER_SIZE> build_packet(int sequence_nb, int start_sequence_nb, int end_sequence_nb,
                                     Flags flag, const asio::ip::udp::endpoint& endpoint,
                                     const std::array<char, BUFFER_SIZE>& message) {
        packet<BUFFER_SIZE> pkt;
        pkt.sequence_nb       = sequence_nb;
        pkt.start_sequence_nb = start_sequence_nb;
        pkt.end_sequence_nb   = end_sequence_nb;

        // Calculate packet<BUFFER_SIZE> size, ensuring it doesn't exceed BUFFER_SIZE
        // pkt.packet_size = std::min(BUFFER_SIZE, static_cast<int>(message.size())); error via the
        // new buffer_size
        pkt.packet_size = std::min(static_cast<std::size_t>(BUFFER_SIZE), message.size());

        size_t start_idx = sequence_nb * BUFFER_SIZE;
        size_t end_idx   = start_idx + pkt.packet_size;

        if (start_idx > message.size()) {
            std::cerr << "Error: Start index out of range!" << std::endl;
            std::cerr << "Start index: " << start_idx << " Message size: " << message.size()
                      << std::endl;
            return {};  // Return an empty packet<BUFFER_SIZE> or handle the error
        }

        // Clear existing data in pkt.data and copy relevant bytes
        pkt.data.fill(0);  // Ensure pkt.data is zero-initialized
        std::copy(message.begin() + start_idx, message.begin() + std::min(end_idx, message.size()),
                  pkt.data.begin());

        pkt.flag     = flag;
        pkt.endpoint = endpoint;
        return pkt;
    }
    // threads function
    void receive() {
        socket_.async_receive_from(asio::buffer(recv_buffer_), _endpoint,
                                   [this](std::error_code ec, std::size_t bytes_recvd) {
                                       if (!ec && bytes_recvd > 0) {
                                           handle_receive(bytes_recvd);
                                       } else {
                                           if (ec) {
                                               std::cerr << "Receive error: " << ec.message()
                                                         << std::endl;
                                           } else {
                                               std::cerr << "Received 0 bytes" << std::endl;
                                           }
                                       }
                                       receive();
                                   });
    }
    void send() {
        while (!_stop_processing) {
            std::unique_lock<std::mutex> lock(_send_queue_mutex);
            _send_queue_cv.wait(lock, [this] { return !_send_queue.empty() || _stop_processing; });
            while (!_send_queue.empty()) {
                auto pkt = _send_queue.front();
                _send_queue.pop_front();
                _send_queue_set.erase(pkt);
                lock.unlock();
                send_packet(pkt);
                lock.lock();
            }
        }
    }

    void retry() {
        while (!_stop_processing) {
            // 150 ms is the time to wait before retrying to send a packet<BUFFER_SIZE> - Windows
            // may need to disable firewall
            std::this_thread::sleep_for(std::chrono::milliseconds(150));
            {
                std::lock_guard<std::mutex> lock(_retry_queue_mutex);
                for (const auto& pkt : _retry_queue) {
                    send_packet(pkt);
                }
            }
        }
    }

    // receive functions
    void handle_receive(std::size_t bytes_recvd) {
        // auto message = std::make_shared<std::string>(recv_buffer_.data(), bytes_recvd);
        // packet<BUFFER_SIZE> pkt     = deserialize_packet(std::vector<char>(message->begin(),
        // message->end())); probably need to change this to std::array<char, BUFFER_SIZE> auto
        // message = std::make_shared<std::vector<char>>(recv_buffer_.begin(),
        //                                                      recv_buffer_.begin() + bytes_recvd);
        // packet<BUFFER_SIZE> pkt     = deserialize_packet(*message);
        auto message = std::make_shared<std::array<char, BUFFER_SIZE>>();
        std::copy(recv_buffer_.begin(), recv_buffer_.begin() + bytes_recvd, message->begin());

        packet<BUFFER_SIZE> pkt = deserialize_packet(*message);
        std::cout << "Received packet<BUFFER_SIZE> from " << pkt.endpoint << std::endl;
        std::cout << "flag: " << pkt.flag << std::endl;
        switch (pkt.flag) {
            case ACK:
                handle_ack(pkt);
                break;
            case RELIABLE:
                handle_reliable_packet(pkt);
                break;
            case UNRELIABLE:
                // handle_unreliable_packet(std::string(pkt.data.begin(), pkt.data.end()));
                handle_unreliable_packet(pkt.data);
                break;
            case NEW_CLIENT:
                handle_new_client(_endpoint);
                break;
            case TEST:
                handle_test(_endpoint);
                break;
            default:
                std::cerr << "Received unknown packet<BUFFER_SIZE> type: " << pkt.flag << std::endl;
                break;
        }
    }
    // handle messages
    // void handle_ack(const std::string& ack_message) {
    //     SEQUENCE_TYPE sequence_start  = 0;
    //     SEQUENCE_TYPE sequence_number = 0;

    //     if (ack_message.size() < 4) {
    //         std::cerr << "Invalid ACK message size" << std::endl;
    //         return;
    //     }
    //     if (ack_message.substr(0, 11) == "CLIENT_ACK:") {
    //         sequence_start  = std::stoi(ack_message.substr(11, ack_message.find(',')));
    //         sequence_number = std::stoi(ack_message.substr(ack_message.find(',') + 1));
    //     } else if (ack_message.substr(0, 4) == "ACK:") {
    //         sequence_start  = std::stoi(ack_message.substr(4, ack_message.find(',')));
    //         sequence_number = std::stoi(ack_message.substr(ack_message.find(',') + 1));
    //     } else {
    //         std::cerr << "Invalid ACK message: " << ack_message << std::endl;
    //         return;
    //     }
    //     {
    //         std::lock_guard<std::mutex> lock(_retry_queue_mutex);
    //         for (auto it = _retry_queue.begin(); it != _retry_queue.end();) {
    //             if (it->sequence_nb == sequence_number && it->start_sequence_nb ==
    //             sequence_start) {
    //                 _retry_queue.erase(it);
    //                 break;
    //             }
    //             ++it;
    //         }
    //     }
    // }
    void handle_ack(const packet<BUFFER_SIZE>& pkt) {
        AckMessage ackMessage = deserialize_ack(pkt.data);

        SEQUENCE_TYPE sequence_start  = ackMessage.start_sequence_number;
        SEQUENCE_TYPE sequence_number = ackMessage.sequence_number;

        {
            std::lock_guard<std::mutex> lock(_retry_queue_mutex);
            for (auto it = _retry_queue.begin(); it != _retry_queue.end();) {
                if (it->sequence_nb == sequence_number && it->start_sequence_nb == sequence_start) {
                    _retry_queue.erase(it);
                    std::cout << "Retry queue size: " << _retry_queue.size() << std::endl;
                    break;
                }
                ++it;
            }
        }
    }

    void handle_reliable_packet(const packet<BUFFER_SIZE>& pkt) {
        bool all_packets_received = false;

        if (pkt.sequence_nb > pkt.end_sequence_nb) {
            std::cerr << "Invalid sequence number: " << pkt.sequence_nb << std::endl;
            return;
        }

        {
            std::lock_guard<std::mutex> lock(_received_packets_mutex);
            // check that the packet<BUFFER_SIZE> is not already in the map
            std::vector<packet<BUFFER_SIZE>> temp_vector = _received_packets[pkt.start_sequence_nb];
            if (std::find(temp_vector.begin(), temp_vector.end(), pkt) != temp_vector.end()) {
            } else {
                _received_packets[pkt.start_sequence_nb].push_back(pkt);
            }

            int final_size    = pkt.end_sequence_nb - pkt.start_sequence_nb + 1;
            int received_size = _received_packets[pkt.start_sequence_nb].size();

            if (received_size == final_size) {
                all_packets_received = true;
            }
        }

        if (all_packets_received) {
            std::array<char, BUFFER_SIZE> complete_data = {};
            size_t                        current_index = 0;

            {
                std::lock_guard<std::mutex> lock(_received_packets_mutex);
                // WARNING: sort could make the program slower
                std::sort(_received_packets[pkt.start_sequence_nb].begin(),
                          _received_packets[pkt.start_sequence_nb].end(),
                          [](const packet<BUFFER_SIZE>& a, const packet<BUFFER_SIZE>& b) {
                              return a.sequence_nb < b.sequence_nb;
                          });

                // for (const auto& packet<BUFFER_SIZE> : _received_packets[pkt.start_sequence_nb])
                // {
                //     complete_data.insert(complete_data.end(), packet<BUFFER_SIZE>.data.begin(),
                //     packet<BUFFER_SIZE>.data.end());
                // }
                // _received_packets.erase(pkt.start_sequence_nb);
                // const std::string message = std::string(complete_data.begin(),
                // complete_data.end());
                for (const auto& packet : _received_packets[pkt.start_sequence_nb]) {
                    size_t data_size = std::min(packet.data.size(), BUFFER_SIZE - current_index);
                    std::copy(packet.data.begin(), packet.data.begin() + data_size,
                              complete_data.begin() + current_index);
                    current_index += data_size;

                    if (current_index >= BUFFER_SIZE) {
                        break;  // Prevent exceeding the fixed size
                    }
                    _received_packets.erase(pkt.start_sequence_nb);
                }

                std::lock_guard<std::mutex> unprocessed_lock(
                    _unprocessed_reliable_messages_data_mutex);
                // Push the complete data and endpoint into the reliable messages vector
                _unprocessed_reliable_messages_data.push_back(
                    std::make_pair(complete_data, _endpoint));

                // Process the message content
                // std::lock_guard<std::mutex>
                // unprocessed_lock(_unprocessed_reliable_messages_mutex);
                // // For now: we will push with the last _endpoint. Can cause issues in the future
                // _unprocessed_reliable_messages.push_back(std::make_pair(message, _endpoint));
            }
        }

        send_ack(pkt.start_sequence_nb, pkt.sequence_nb, _endpoint);
    }

    void handle_unreliable_packet(const std::array<char, BUFFER_SIZE>& message) {
        // Process the message content
        std::lock_guard<std::mutex> lock(_unprocessed_unreliable_messages_data_mutex);
        _unprocessed_unreliable_messages_data.push_back(std::make_pair(message, _endpoint));
    }
    // void handle_unreliable_packet(const std::array<char, BUFFER_SIZE>& message);
    void handle_new_client(const asio::ip::udp::endpoint& client_endpoint) {
        if (_known_clients.find(client_endpoint) != _known_clients.end()) return;

        _known_clients.insert(client_endpoint);
        // here we used to send test message. TODO test flag to trigger the test

        if (_new_client_callback) _new_client_callback(client_endpoint);
    }

    void handle_test(const asio::ip::udp::endpoint& endpoint) {
        // send reliable packet<BUFFER_SIZE> to the client with message testPacketManager()
        // send_reliable_packet(testPacketManager(), endpoint);
    }

    // send functions
    void send_ack(SEQUENCE_TYPE start_sequence_number, SEQUENCE_TYPE sequence_number,
                  const asio::ip::udp::endpoint& endpoint_) {
        // std::string ack_message;
        // if (role_ == Role::SERVER) {
        //     ack_message = "CLIENT_ACK:" + std::to_string(start_sequence_number) + "," +
        //                   std::to_string(sequence_number);
        // }
        // if (role_ == Role::CLIENT) {
        //     ack_message = "ACK:" + std::to_string(start_sequence_number) + "," +
        //                   std::to_string(sequence_number);
        // }
        // std::cout << "Sending ack: " << ack_message << std::endl;
        // packet<BUFFER_SIZE> pkt = build_packet(0, 0, 0, ACK, endpoint_, ack_message);
        // queue_packet_for_sending(pkt);

        AckMessage ackMessage = {AckType::ACK, 0, 0};  // Initialize with default values

        // Set the AckType based on the role
        if (role_ == Role::SERVER) {
            ackMessage.ack_type = AckType::CLIENT_ACK;
        } else if (role_ == Role::CLIENT) {
            ackMessage.ack_type = AckType::ACK;
        }

        // Set the sequence numbers
        ackMessage.start_sequence_number = start_sequence_number;
        ackMessage.sequence_number       = sequence_number;

        // Serialize the ackMessage
        std::array<char, BUFFER_SIZE> buffer;

        // Serialize the AckMessage into the buffer
        serialize_ack(ackMessage, buffer);

        // build packet
        packet<BUFFER_SIZE> pkt = build_packet(0, 0, 0, ACK, endpoint_, buffer);

        // Send the buffer over the network using your packet manager or UDP socket
        queue_packet_for_sending(pkt);
    }

    void queue_packet_for_sending(const packet<BUFFER_SIZE>& pkt) {
        {
            std::lock_guard<std::mutex> lock(_send_queue_mutex);
            if (_send_queue_set.find(pkt) == _send_queue_set.end()) {
                _send_queue.emplace_back(pkt);
                std::cout << "send_queue size: " << _send_queue.size() << std::endl;
                _send_queue_set.insert(pkt);
            } else {
            }
        }
        _send_queue_cv.notify_one();
    }

    void send_packet(const packet<BUFFER_SIZE>& pkt) {
        const auto buffer = std::make_shared<std::vector<char>>(serialize_packet(pkt));
        std::this_thread::sleep_for(std::chrono::nanoseconds(100000));
        socket_.async_send_to(asio::buffer(*buffer), pkt.endpoint,
                              [this, buffer](std::error_code ec, std::size_t bytes_sent) {
                                  if (ec) {
                                      std::cerr << "Send error: " << ec.message()
                                                << " size: " << bytes_sent << std::endl;
                                  }
                              });
    }
    void send_reliable_packet(const std::array<char, BUFFER_SIZE>& message,
                              const asio::ip::udp::endpoint&       endpoint) {
        int total_packets = (message.size() + BUFFER_SIZE - 1) / BUFFER_SIZE;

        int start_sequence_nb;
        int end_sequence_nb;

        {
            std::lock_guard<std::mutex> lock(_message_id_mutex);
            start_sequence_nb = _message_id;
            end_sequence_nb   = _message_id + total_packets - 1;
        }

        for (int i = 0; i < total_packets; ++i) {
            packet<BUFFER_SIZE> pkt;

            {
                std::lock_guard<std::mutex> lock(_message_id_mutex);
                pkt = build_packet(_message_id - start_sequence_nb, start_sequence_nb,
                                   end_sequence_nb, RELIABLE, endpoint, message);
                _message_id++;
            }

            if (endpoint.address().is_unspecified()) {
                std::cerr << "Server endpoint is unspecified" << std::endl;
                return;
            }
            if (pkt.sequence_nb > pkt.end_sequence_nb) {
                std::cerr << "sending reliable Invalid sequence number: " << pkt.sequence_nb
                          << " start_sequence_nb: " << pkt.start_sequence_nb
                          << " end_sequence_nb: " << pkt.end_sequence_nb << std::endl;
                continue;
            }
            queue_packet_for_sending(pkt);
            queue_packet_for_retry(pkt);
        }
    }
    void send_new_client(const asio::ip::udp::endpoint& endpoint) {
        packet<BUFFER_SIZE> pkt = build_packet(0, 0, 0, NEW_CLIENT, endpoint, {0});
        queue_packet_for_sending(pkt);
    }
    void send_test(const asio::ip::udp::endpoint& endpoint) {
        packet<BUFFER_SIZE> pkt = build_packet(0, 0, 0, TEST, endpoint, {0});
        queue_packet_for_sending(pkt);
    }

    void send_unreliable_packet(const std::array<char, BUFFER_SIZE>& message,
                                const asio::ip::udp::endpoint&       endpoint) {
        packet<BUFFER_SIZE> pkt = build_packet(0, 0, 0, UNRELIABLE, endpoint, message);
        queue_packet_for_sending(pkt);
    }

    // retry functions
    void queue_packet_for_retry(const packet<BUFFER_SIZE>& pkt) {
        {
            std::lock_guard<std::mutex> lock(_retry_queue_mutex);
            _retry_queue.push_back(pkt);
            std::cout << "retry_queue size: " << _retry_queue.size() << std::endl;
        }
    }

    // void schedule_retransmissions(const asio::ip::udp::endpoint& endpoint);
    // std::queue<packet> get_received_packets();
    std::unordered_set<asio::ip::udp::endpoint, EndpointHash, EndpointEqual> getKnownClients() {
        return _known_clients;
    }
    // overload for vector<char>
    const std::array<char, BUFFER_SIZE> get_last_reliable_packet_data() {
        std::lock_guard<std::mutex> lock(_unprocessed_reliable_messages_data_mutex);
        if (_unprocessed_reliable_messages_data.empty()) return {};
        std::array<char, BUFFER_SIZE> message;
        std::copy(_unprocessed_reliable_messages_data.back().first.begin(),
                  _unprocessed_reliable_messages_data.back().first.end(), message.begin());
        _unprocessed_reliable_messages_data.erase(_unprocessed_reliable_messages_data.end() - 1);
        std::cout << "size of unprocessed_reliable_messages_data: "
                  << _unprocessed_reliable_messages_data.size() << std::endl;
        return message;
    }
    const std::array<char, BUFFER_SIZE> get_last_unreliable_packet_data() {
        std::lock_guard<std::mutex> lock(_unprocessed_unreliable_messages_data_mutex);
        if (_unprocessed_unreliable_messages_data.empty()) return {};
        std::array<char, BUFFER_SIZE> message;
        std::copy(_unprocessed_unreliable_messages_data.back().first.begin(),
                  _unprocessed_unreliable_messages_data.back().first.end(), message.begin());
        _unprocessed_unreliable_messages_data.erase(_unprocessed_unreliable_messages_data.end() -
                                                    1);
        return message;
    }

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
        const asio::ip::udp::endpoint& endpoint) {
        std::vector<std::array<char, BUFFER_SIZE>> messages;
        std::lock_guard<std::mutex> lock(_unprocessed_unreliable_messages_data_mutex);
        for (auto it = _unprocessed_unreliable_messages_data.begin();
             it != _unprocessed_unreliable_messages_data.end();) {
            if (it->second == endpoint) {
                std::array<char, BUFFER_SIZE> message_array;
                std::copy(it->first.begin(), it->first.end(), message_array.begin());
                messages.push_back(message_array);
                it = _unprocessed_unreliable_messages_data.erase(it);
            } else {
                ++it;
            }
        }
        return messages;
    }
    std::vector<std::array<char, BUFFER_SIZE>> get_reliable_messages_from_endpoint_data(
        const asio::ip::udp::endpoint& endpoint) {
        std::vector<std::array<char, BUFFER_SIZE>> messages;
        std::lock_guard<std::mutex>                lock(_unprocessed_reliable_messages_data_mutex);
        for (auto it = _unprocessed_reliable_messages_data.begin();
             it != _unprocessed_reliable_messages_data.end();) {
            if (it->second == endpoint) {
                std::array<char, BUFFER_SIZE> message_array;
                std::copy(it->first.begin(), it->first.end(), message_array.begin());
                messages.push_back(message_array);
                it = _unprocessed_reliable_messages_data.erase(it);
            } else {
                ++it;
            }
        }
        return messages;
    }

    // void               retransmit_unacknowledged_packets(const asio::ip::udp::endpoint&
    // endpoint);
    void print_packet(const packet<BUFFER_SIZE>& pkt) {
        std::cout << "Packet: " << std::endl;
        std::cout << "Sequence number: " << pkt.sequence_nb << std::endl;
        std::cout << "Start sequence number: " << pkt.start_sequence_nb << std::endl;
        std::cout << "End sequence number: " << pkt.end_sequence_nb << std::endl;
        std::cout << "Packet size: " << pkt.packet_size << std::endl;
        std::cout << "Flag: " << pkt.flag << std::endl;
        std::cout << "Data: " << std::string(pkt.data.begin(), pkt.data.end()) << std::endl;
    }

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

    std::unordered_map<int, std::vector<packet<BUFFER_SIZE>>> _received_packets;
    std::mutex                                                _received_packets_mutex;

    // send packets variables
    std::deque<packet<BUFFER_SIZE>>         _send_queue;
    std::mutex                              _send_queue_mutex;
    std::condition_variable                 _send_queue_cv;
    std::unordered_set<packet<BUFFER_SIZE>> _send_queue_set;

    // retry packets variables
    std::deque<packet<BUFFER_SIZE>> _retry_queue;
    std::mutex                      _retry_queue_mutex;

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