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
#include <unordered_map>
#include "PacketUtils.hpp"

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

    void start_receive() {
        socket_.async_receive_from(asio::buffer(recv_buffer_), remote_endpoint_,
                                   [this](std::error_code ec, std::size_t bytes_recvd) {
                                       if (!ec && bytes_recvd > 0) {
                                           handle_receive(bytes_recvd);
                                       }
                                       start_receive();
                                   });
    }

    void handle_receive(std::size_t bytes_recvd) {
        auto   message = std::make_shared<std::string>(recv_buffer_.data(), bytes_recvd);
        packet pkt     = deserialize_packet(std::vector<char>(message->begin(), message->end()));

        {
            std::lock_guard<std::mutex> lock(queue_mutex_);
            packet_queue_.push(pkt);
        }
        queue_cv_.notify_one();
    }

    void process_packets() {
        while (!stop_processing_) {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            queue_cv_.wait(lock, [this] { return !packet_queue_.empty() || stop_processing_; });

            while (!packet_queue_.empty()) {
                packet pkt = packet_queue_.front();
                packet_queue_.pop();
                switch (pkt.flag) {
                case ACK:
                    handle_ack(std::string(pkt.data.begin(), pkt.data.end()));
                    break;
                case RELIABLE:
                    // Process the reliable packet
                    handle_reliable_packet(pkt);
                    break;
                case UNRELIABLE:
                    // Process the unreliable packet
                    handle_unreliable_packet(std::string(pkt.data.begin(), pkt.data.end()));
                    break;
                default:
                    std::cerr << "Received unknown packet type: " << pkt.flag << std::endl;
                    break;
                }
                if (Role::SERVER == role_) {
                    handle_new_client(remote_endpoint_);
                }
            }
        }
    }

    void handle_new_client(const asio::ip::udp::endpoint& client_endpoint) {
        if (known_clients_.find(client_endpoint) == known_clients_.end()) {
            known_clients_.insert(client_endpoint);
            std::cout << "New client connected: " << client_endpoint << std::endl;

            std::string boat_info =
                "Boats are watercraft of various sizes designed to float, plane, work, or travel "
                "on "
                "water. "
                "They are typically smaller than ships, which are generally distinguished by their "
                "larger size, "
                "shape, cargo or passenger capacity, or their ability to carry boats. "
                "Boats have been used since prehistoric times and have been essential for fishing, "
                "transportation, "
                "trade, and warfare. Modern boats are usually powered by engines, but many still "
                "use "
                "sails or oars. "
                "There are many types of boats, including sailboats, motorboats, fishing boats, "
                "and "
                "rowboats. ";

            // Repeat the boat_info string to exceed 10,000 bytes
            std::string long_boat_info;
            while (long_boat_info.size() <= 10000000) {
                long_boat_info += boat_info;
            }
            // Send test packets to the new client
            // send_unreliable_packet("Unreliable packet from server", client_endpoint);
            // send_reliable_packet("Reliable packet from server", client_endpoint);
            send_reliable_packet(long_boat_info, client_endpoint);
        }
    }

    void handle_reliable_packet(const packet& pkt) {
        std::lock_guard<std::mutex> lock(unacknowledged_packets_mutex_);
        // Process the message content
        // Print the received packet details
        // std::cout << "Received packet with sequence number: " << pkt.sequence_no <<
        // std::endl; std::cout << "Packet size: " << pkt.packet_size << std::endl; std::cout <<
        // "Data: " << std::string(pkt.data.begin(), pkt.data.end()) << std::endl;

        // Store the packet
        received_packets_[pkt.sequence_no] = pkt;

        // Set start and max sequence numbers
        if (start_sequence_no_ == -1) {
            start_sequence_no_ = pkt.start_sequence_no;
            end_sequence_no    = pkt.end_sequence_no;
        }

        // Check if all packets have been received
        int final_size    = end_sequence_no - start_sequence_no_ + 1;
        int received_size = received_packets_.size();
        std::cout << "Received packets: " << received_size << " out of " << final_size << " packets"
                  << std::endl;
        if (received_size == final_size) {
            std::cout << "Reassembling message" << std::endl;
            // All packets received, reassemble the message
            std::vector<char> complete_data;
            for (int i = start_sequence_no_; i <= end_sequence_no; ++i) {
                const auto& pkt = received_packets_[i];
                complete_data.insert(complete_data.end(), pkt.data.begin(), pkt.data.end());
            }

            std::string complete_message(complete_data.begin(), complete_data.end());
            std::cout << "Complete message: " << complete_message << std::endl;
            std::cout << "Total size received: " << complete_data.size() << " bytes" << std::endl;

            // Clear the received packets map for the next message
            received_packets_.clear();
            start_sequence_no_ = -1;
            end_sequence_no    = -1;
        }

        // Send ACK back to the server
        send_ack(pkt.sequence_no, remote_endpoint_);
    }

    void handle_unreliable_packet(const std::string& message) {
        // Process the message content
        std::cout << "Processing unreliable message: " << message << std::endl;
    }

    void send_packets() {
        while (!stop_processing_) {
            std::unique_lock<std::mutex> lock(send_queue_mutex_);
            send_queue_cv_.wait(lock, [this] { return !send_queue_.empty() || stop_processing_; });

            while (!send_queue_.empty()) {
                auto [pkt, endpoint] = send_queue_.front();
                send_queue_.pop();
                lock.unlock(); // Unlock the mutex while sending the packet

                send_packet(pkt, endpoint);

                lock.lock(); // Lock the mutex again before checking the queue
            }
        }
    }

    void handle_retransmissions() {
        while (!stop_processing_) {
            std::unique_lock<std::mutex> lock(retransmission_queue_mutex_);
            retransmission_queue_cv_.wait(
                lock, [this] { return !retransmission_queue_.empty() || stop_processing_; });

            while (!retransmission_queue_.empty()) {
                auto [pkt, endpoint] = retransmission_queue_.front();
                retransmission_queue_.pop();
                lock.unlock(); // Unlock the mutex while sending the packet

                send_packet(pkt, endpoint);
                std::cout << "retransmissionqueue.size(): " << retransmission_queue_.size()
                          << std::endl;

                lock.lock(); // Lock the mutex again before checking the queue
            }
        }
    }

    void queue_packet_for_retransmission(const packet&                  pkt,
                                         const asio::ip::udp::endpoint& endpoint) {
        {
            std::lock_guard<std::mutex> lock(retransmission_queue_mutex_);
            retransmission_queue_.emplace(pkt, endpoint);
        }
        retransmission_queue_cv_.notify_one();
    }

    void schedule_retransmissions(const asio::ip::udp::endpoint& endpoint) {
        retransmission_timer_.expires_after(std::chrono::milliseconds(300));
        retransmission_timer_.async_wait([this, endpoint](const std::error_code& ec) {
            if (!ec) {
                std::lock_guard<std::mutex> lock(unacknowledged_packets_mutex_);
                for (const auto& pair : unacknowledged_packets_) {
                    queue_packet_for_retransmission(pair.second, endpoint);
                }
                if (!unacknowledged_packets_.empty()) {
                    schedule_retransmissions(endpoint);
                }
            }
        });
    }

    void handle_ack(const std::string& ack_message) {
        std::uint32_t sequence_number = 0;
        // std::cout << "ack_message: " << ack_message << std::endl;
        if (ack_message.size() < 4) {
            std::cerr << "Invalid ACK message: " << ack_message << std::endl;
            return;
        }
        if (ack_message.substr(0, 11) == "CLIENT_ACK:") {
            sequence_number = std::stoul(ack_message.substr(11));
        } else if (ack_message.substr(0, 4) == "ACK:") {
            sequence_number = std::stoul(ack_message.substr(4));
        } else {
            std::cerr << "Invalid ACK message: " << ack_message << std::endl;
            return;
        }
        // std::cout << "Received ACK for sequence number: " << sequence_number << std::endl;
        // unacknowledged_packets_.erase(sequence_number);
        {
            std::lock_guard<std::mutex> lock(unacknowledged_packets_mutex_);
            unacknowledged_packets_.erase(sequence_number);
        }
    }

    void send_reliable_packet(const std::string& message, const asio::ip::udp::endpoint& endpoint) {
        int total_packets = (message.size() + BUFFER_SIZE - 1) / BUFFER_SIZE;
        std::cout << "Sending " << total_packets << " packets" << " size: " << message.size()
                  << std::endl;

        int start_sequence_no = sequence_number_;
        int end_sequence_no   = sequence_number_ + total_packets - 1;

        for (int i = 0; i < total_packets; ++i) {
            packet pkt;
            pkt.sequence_no       = sequence_number_++;
            pkt.start_sequence_no = start_sequence_no;
            pkt.end_sequence_no   = end_sequence_no;
            pkt.packet_size =
                std::min(BUFFER_SIZE, static_cast<int>(message.size() - i * BUFFER_SIZE));
            pkt.flag = RELIABLE;
            pkt.data.assign(message.begin() + i * BUFFER_SIZE,
                            message.begin() + i * BUFFER_SIZE + pkt.packet_size);
            unacknowledged_packets_[pkt.sequence_no] = pkt;
            // send_packet(pkt, endpoint);
            queue_packet_for_sending(pkt, endpoint);
        }
        schedule_retransmissions(endpoint);
    }

    void queue_packet_for_sending(const packet& pkt, const asio::ip::udp::endpoint& endpoint) {
        {
            std::lock_guard<std::mutex> lock(send_queue_mutex_);
            send_queue_.emplace(pkt, endpoint);
        }
        send_queue_cv_.notify_one();
    }

    void send_unreliable_packet(const std::string&             message,
                                const asio::ip::udp::endpoint& endpoint) {
        packet pkt;
        pkt.sequence_no = 0; // no validation for unreliable packets
        pkt.packet_size = message.size();
        pkt.flag        = UNRELIABLE;
        pkt.data.assign(message.begin(), message.end());

        // Serialize the packet using the serialize_packet method
        queue_packet_for_sending(pkt, endpoint);
        // const auto buffer = std::make_shared<std::vector<char>>(serialize_packet(pkt));
        // socket_.async_send_to(asio::buffer(*buffer), endpoint,
        //                       [this, buffer](std::error_code ec, std::size_t bytes_sent) {
        //                           if (ec) {
        //                               std::cerr << "Send error: " << ec.message()
        //                                         << " size: " << bytes_sent << std::endl;
        //                           }
        //                       });
    }

    void send_ack(std::uint32_t sequence_number, const asio::ip::udp::endpoint& endpoint_) {
        std::string ack_message;
        if (role_ == Role::SERVER) {
            ack_message = "CLIENT_ACK:" + std::to_string(sequence_number);
        }
        if (role_ == Role::CLIENT) {
            ack_message = "ACK:" + std::to_string(sequence_number);
        }
        packet pkt;
        pkt.sequence_no = sequence_number;
        pkt.packet_size = ack_message.size();
        pkt.flag        = ACK;
        pkt.data.assign(ack_message.begin(), ack_message.end());

        queue_packet_for_sending(pkt, endpoint_);
        // Serialize the packet using the serialize_packet method
        // const auto buffer = std::make_shared<std::vector<char>>(serialize_packet(pkt));
        // std::cout << "Sending ACK for sequence number: " << sequence_number << " to " <<
        // endpoint_
        //           << std::endl;

        if (endpoint_.address().is_unspecified()) {
            std::cerr << "Server endpoint is unspecified" << std::endl;
            return;
        }

        // socket_.async_send_to(asio::buffer(*buffer), endpoint_,
        //                       [this, buffer](std::error_code ec, std::size_t bytes_sent) {
        //                           if (ec) {
        //                               std::cerr << "Send ACK error: " << ec.message()
        //                                         << " size: " << bytes_sent << std::endl;
        //                           }
        //                       });
    }

    std::queue<packet> get_received_packets() {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        return packet_queue_;
    }

  private:
    void send_packet(const packet& pkt, const asio::ip::udp::endpoint& endpoint) {
        // Serialize the packet using the serialize_packet method
        const auto buffer = std::make_shared<std::vector<char>>(serialize_packet(pkt));
        socket_.async_send_to(asio::buffer(*buffer), endpoint,
                              [this, buffer](std::error_code ec, std::size_t bytes_sent) {
                                  if (ec) {
                                      std::cerr << "Send error: " << ec.message()
                                                << " size: " << bytes_sent << std::endl;
                                  }
                              });
    }

    void retransmit_unacknowledged_packets(const asio::ip::udp::endpoint& endpoint) {
        for (const auto& pair : unacknowledged_packets_) {
            std::cout << "unacknowledged packet.size(): " << unacknowledged_packets_.size()
                      << std::endl;

            // (pair.second, endpoint);
        }
    }

    // void schedule_retransmissions(const asio::ip::udp::endpoint& endpoint) {
    //     retransmission_timer_.expires_after(std::chrono::milliseconds(300));
    //     retransmission_timer_.async_wait([this, endpoint](const std::error_code& ec) {
    //         if (!ec) {
    //             retransmit_unacknowledged_packets(endpoint);
    //             for (const auto& pair : unacknowledged_packets_) {
    //                 std::cout << "Unacknowledged packet: " << pair.first << std::endl;
    //             }
    //             if (!unacknowledged_packets_.empty()) {
    //                 schedule_retransmissions(endpoint);
    //             }
    //         }
    //     });
    // }

    // TODO change to a MACRO for changing the sequence number max allocated size
    std::uint32_t                             sequence_number_;
    std::unordered_map<std::uint32_t, packet> unacknowledged_packets_;
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

    std::vector<std::thread> thread_pool_;
    std::size_t              thread_pool_size_;

    std::queue<std::pair<packet, asio::ip::udp::endpoint>> send_queue_;
    std::mutex                                             send_queue_mutex_;
    std::condition_variable                                send_queue_cv_;
    std::mutex                                             unacknowledged_packets_mutex_;

    std::queue<std::pair<packet, asio::ip::udp::endpoint>> retransmission_queue_;
    std::mutex                                             retransmission_queue_mutex_;
    std::condition_variable                                retransmission_queue_cv_;
    std::thread                                            retransmission_thread_;

    // work guard
    asio::executor_work_guard<asio::io_context::executor_type> work_guard_;
};

#endif // SENDPACKETS_HPP