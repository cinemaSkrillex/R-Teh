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
#include <unordered_map>
#include "../ClientExport.hpp"
#include "../shared/PacketUtils.hpp"

enum class Role { SERVER, CLIENT };

class PacketManager {
  public:
    PacketManager(asio::io_context& io_context, asio::ip::udp::socket& socket, Role role)
        : sequence_number_(0), retransmission_timer_(io_context), socket_(socket), role_(role) {}

    void handle_ack(const std::string& ack_message) {
        std::uint32_t sequence_number = 0;
        std::cout << "ack_message: " << ack_message << std::endl;
        if (ack_message.substr(0, 11) == "CLIENT_ACK:") {
            sequence_number = std::stoul(ack_message.substr(11));
        } else if (ack_message.substr(0, 4) == "ACK:") {
            sequence_number = std::stoul(ack_message.substr(4));
        } else {
            std::cerr << "Invalid ACK message: " << ack_message << std::endl;
            return;
        }
        std::cout << "Received ACK for sequence number: " << sequence_number << std::endl;
        unacknowledged_packets_.erase(sequence_number);
    }
    void send_reliable_packet(const std::string& message, const asio::ip::udp::endpoint& endpoint) {
        int total_packets = (message.size() + BUFFER_SIZE - 1) / BUFFER_SIZE;
        std::cout << "Sending " << total_packets << " packets" << " size: " << message.size()
                  << std::endl;

        for (int i = 0; i < total_packets; ++i) {
            packet pkt;
            pkt.sequence_no = sequence_number_++;
            pkt.packet_size =
                std::min(BUFFER_SIZE, static_cast<int>(message.size() - i * BUFFER_SIZE));
            pkt.flag = RELIABLE;
            pkt.data.assign(message.begin() + i * BUFFER_SIZE,
                            message.begin() + i * BUFFER_SIZE + pkt.packet_size);
            unacknowledged_packets_[pkt.sequence_no] = pkt;
            send_packet(pkt, endpoint);
        }
        schedule_retransmissions(endpoint);
    }

    void send_unreliable_packet(const std::string&             message,
                                const asio::ip::udp::endpoint& endpoint) {
        packet pkt;
        pkt.sequence_no = 0; // no validation for unreliable packets
        pkt.packet_size = message.size();
        pkt.flag        = UNRELIABLE;
        pkt.data.assign(message.begin(), message.end());

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

        // Serialize the packet using the serialize_packet method
        const auto buffer = std::make_shared<std::vector<char>>(serialize_packet(pkt));
        std::cout << "Sending ACK for sequence number: " << sequence_number << " to " << endpoint_
                  << std::endl;

        if (endpoint_.address().is_unspecified()) {
            std::cerr << "Server endpoint is unspecified" << std::endl;
            return;
        }

        socket_.async_send_to(asio::buffer(*buffer), endpoint_,
                              [this, buffer](std::error_code ec, std::size_t bytes_sent) {
                                  if (ec) {
                                      std::cerr << "Send ACK error: " << ec.message()
                                                << " size: " << bytes_sent << std::endl;
                                  }
                              });
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
            std::cout << "Retransmitting packet with sequence number: " << pair.first << std::endl;
            send_packet(pair.second, endpoint);
        }
    }

    void schedule_retransmissions(const asio::ip::udp::endpoint& endpoint) {
        retransmission_timer_.expires_after(std::chrono::milliseconds(300));
        retransmission_timer_.async_wait([this, endpoint](const std::error_code& ec) {
            if (!ec) {
                retransmit_unacknowledged_packets(endpoint);
                for (const auto& pair : unacknowledged_packets_) {
                    std::cout << "Unacknowledged packet: " << pair.first << std::endl;
                }
                if (!unacknowledged_packets_.empty()) {
                    schedule_retransmissions(endpoint);
                }
            }
        });
    }

    std::uint32_t                             sequence_number_;
    std::unordered_map<std::uint32_t, packet> unacknowledged_packets_;
    asio::steady_timer                        retransmission_timer_;
    asio::ip::udp::socket&                    socket_;
    Role                                      role_;
};

#endif // SENDPACKETS_HPP