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
    handle_new_client(remote_endpoint_);
    handle_unreliable_packet(*message);

    start_receive();
}

void UDPServer::handle_reliable_packet(const std::string& message, std::size_t colon_pos) {
    std::uint32_t sequence_number = std::stoul(message.substr(0, colon_pos));
    // Send ACK back to the client
    std::cout << "received UDPServer::handle_reliable_packet" << sequence_number
              << "remote: " << remote_endpoint_ << std::endl;
    // Process the message content
    std::cout << "Processing reliable message: " << message.substr(colon_pos + 1) << std::endl;
    start_receive(); // Continue receiving incoming packets
}

void UDPServer::handle_unreliable_packet(const std::string& message) {
    // Process the message content
    std::cout << "Processing unreliable message: " << message << std::endl;
}

void UDPServer::handle_new_client(const asio::ip::udp::endpoint& client_endpoint) {
    if (known_clients_.find(client_endpoint) == known_clients_.end()) {
        known_clients_.insert(client_endpoint);
        std::cout << "New client connected: " << client_endpoint << std::endl;

        // Send test packets to the new client
        send_unreliable_packet("Unreliable packet", client_endpoint);
        send_reliable_packet("Reliable packet", client_endpoint);
    }
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
    // TO DO: Implement reliable packet sending
}

extern "C" SERVER_API UDPServer* create_udp_server(asio::io_context& io_context,
                                                   unsigned short    port) {
    return new UDPServer(io_context, port);
}