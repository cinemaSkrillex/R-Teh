/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** UDPClient.cpp
*/

#include "../../include/Client/UDPClient.hpp"

UDPClient::UDPClient(asio::io_context& io_context, unsigned short port)
    : socket_(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)) {
    start_receive();
}

void UDPClient::start_receive() {
    socket_.async_receive_from(asio::buffer(recv_buffer_), server_endpoint_,
                               [this](std::error_code ec, std::size_t bytes_recvd) {
                                   if (!ec && bytes_recvd > 0) {
                                       std::string message(recv_buffer_.data(), bytes_recvd);
                                       std::cout << "Received: " << message << std::endl;

                                       // Check if the message contains a colon
                                       std::size_t colon_pos = message.find(':');
                                       if (colon_pos != std::string::npos) {
                                           handle_reliable_packet(message, colon_pos);
                                       } else {
                                           handle_unreliable_packet(message);
                                       }
                                   }
                                   start_receive();
                               });
}

void UDPClient::handle_reliable_packet(const std::string& message, std::size_t colon_pos) {
    std::uint32_t sequence_number = std::stoul(message.substr(0, colon_pos));
    // Send ACK back to the server
    std::cout << "Sending ACK for sequence number: " << sequence_number << std::endl;
    // Process the message content
    std::cout << "Processing reliable message: " << message.substr(colon_pos + 1) << std::endl;
}

void UDPClient::handle_unreliable_packet(const std::string& message) {
    // Process the message content
    std::cout << "Processing unreliable message: " << message << std::endl;
}

void UDPClient::send_unreliable_packet(const std::string&             message,
                                       const asio::ip::udp::endpoint& server_endpoint) {
    socket_.async_send_to(asio::buffer(message), server_endpoint,
                          [this](std::error_code ec, std::size_t bytes_sent) {
                              if (!ec) {
                                  std::cout << "Sent: " << bytes_sent << " bytes" << std::endl;
                              }
                          });
}

void UDPClient::send_reliable_packet(const std::string&             message,
                                     const asio::ip::udp::endpoint& server_endpoint) {
    // Append the sequence number to the message
    std::string message_with_sequence_number  = std::to_string(sequence_number_) + ":" + message;
    unacknowledged_packets_[sequence_number_] = message_with_sequence_number;
    sequence_number_++;

    socket_.async_send_to(asio::buffer(message_with_sequence_number), server_endpoint,
                          [this](std::error_code ec, std::size_t bytes_sent) {
                              if (!ec) {
                                  std::cout << "Sent: " << bytes_sent << " bytes" << std::endl;
                              }
                          });
}

extern "C" CLIENT_API UDPClient* create_udp_client(asio::io_context& io_context,
                                                   unsigned short    port) {
    return new UDPClient(io_context, port);
}