/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** UDPClient.cpp
*/

#include "../../include/Client/UDPClient.hpp"

UDPClient::UDPClient(asio::io_context& io_context, unsigned short port)
    : socket_(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)) {}

void UDPClient::send_ack(const asio::ip::udp::endpoint& server_endpoint,
                         std::uint32_t                  sequence_number) {
    std::string ack_message = "ACK" + std::to_string(sequence_number);
    socket_.async_send_to(asio::buffer(ack_message), server_endpoint,
                          [](std::error_code /*ec*/, std::size_t /*bytes_sent*/) {});
}

void UDPClient::receive_packet() {
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
                                   receive_packet();
                               });
}

void UDPClient::handle_reliable_packet(const std::string& message, std::size_t colon_pos) {
    std::uint32_t sequence_number = std::stoul(message.substr(0, colon_pos));
    // Send ACK back to the server
    std::cout << "Sending ACK for sequence number: " << sequence_number << std::endl;
    send_ack(server_endpoint_, sequence_number);
    // Process the message content
    std::cout << "Processing reliable message: " << message.substr(colon_pos + 1) << std::endl;
}

void UDPClient::handle_unreliable_packet(const std::string& message) {
    // Process the message content
    std::cout << "Processing unreliable message: " << message << std::endl;
}