/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** UDPServer.cpp
*/

#include "../../include/Server/UDPServer.hpp"
#include <bits/this_thread_sleep.h>

UDPServer::UDPServer(asio::io_context& io_context, unsigned short port)
    : socket_(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)) {
    start_receive();
}

void UDPServer::start_receive() {
    socket_.async_receive_from(asio::buffer(recv_buffer_), remote_endpoint_,
                               [this](std::error_code ec, std::size_t bytes_recvd) {
                                   if (!ec && bytes_recvd > 0) {
                                       handle_receive(bytes_recvd);
                                   } else {
                                       start_receive();
                                   }
                               });
}

void UDPServer::handle_receive(std::size_t bytes_recvd) {
    // Create a persistent std::shared_ptr for the message
    auto message = std::make_shared<std::string>(recv_buffer_.data(), bytes_recvd);
    std::cout << "Received: " << *message << std::endl;

    if (message->substr(0, 3) == "ack") {
        handle_ack(*message);
    } else if (*message == "ping") {
        // Create a persistent std::shared_ptr for the response
        auto response = std::make_shared<std::string>("pong");
        socket_.async_send_to(asio::buffer(*response), remote_endpoint_,
                              [this, response](std::error_code /*ec*/, std::size_t /*bytes_sent*/) {
                                  start_receive();
                              });
    } else {
        start_receive();
    }
}

void UDPServer::handle_ack(const std::string& ack_message) {
    std::uint32_t ack_sequence_number = std::stoul(ack_message.substr(3));
    unacknowledged_packets_.erase(ack_sequence_number);
    start_receive(); // Continue receiving incoming packets
}

/**
 * @brief Sends an unreliable packet to the specified endpoint.
 *
 * This function sends a packet containing the given message to the specified
 * endpoint using UDP. The packet is sent asynchronously and is considered
 * unreliable, meaning it is not critical if the packet is lost during
 * transmission (e.g., player position updates).
 *
 * @param message The message to be sent in the packet.
 * @param endpoint The destination endpoint to which the packet will be sent.
 */
void UDPServer::send_unreliable_packet(const std::string&             message,
                                       const asio::ip::udp::endpoint& endpoint) {
    auto packet = std::make_shared<std::string>(message);
    socket_.async_send_to(asio::buffer(*packet), endpoint,
                          [this, packet](std::error_code /*ec*/, std::size_t /*bytes_sent*/) {});
}

/**
 * @brief Sends a reliable packet over UDP.
 *
 * This function sends a packet to the specified endpoint and ensures that the packet is reliably
 * delivered by implementing an acknowledgment mechanism. If the packet is not acknowledged within
 * a certain time frame, it will be resent.
 *
 * @param message The message to be sent.
 * @param endpoint The destination endpoint to which the message should be sent.
 */
void UDPServer::send_reliable_packet(const std::string&             message,
                                     const asio::ip::udp::endpoint& endpoint) {
    // std::uint6_t to avoid overflow
    std::uint32_t current_sequence_number = sequence_number_++;
    std::string   packet                  = std::to_string(current_sequence_number) + ":" + message;
    unacknowledged_packets_[current_sequence_number] = packet;

    auto send_packet = [this, packet, endpoint, current_sequence_number]() {
        auto packet_ptr = std::make_shared<std::string>(packet);
        socket_.async_send_to(asio::buffer(*packet_ptr), endpoint,
                              [this, packet_ptr, current_sequence_number,
                               endpoint](std::error_code /*ec*/, std::size_t /*bytes_sent*/) {
                                  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                                  if (unacknowledged_packets_.find(current_sequence_number) !=
                                      unacknowledged_packets_.end()) {
                                      send_reliable_packet(
                                          packet_ptr->substr(packet_ptr->find(":") + 1), endpoint);
                                  }
                              });
    };

    send_packet();
}

extern "C" SERVER_API UDPServer* create_udp_server(asio::io_context& io_context,
                                                   unsigned short    port) {
    return new UDPServer(io_context, port);
}