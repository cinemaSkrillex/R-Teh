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
    socket_.async_receive_from(
        asio::buffer(recv_buffer_), server_endpoint_,
        [this](std::error_code ec, std::size_t bytes_recvd) {
            if (!ec && bytes_recvd > 0) {
                if (bytes_recvd >=
                    sizeof(int) *
                        2) { // Check if the message is large enough to be a reliable packet
                    std::cout << "Received " << bytes_recvd << " bytes" << std::endl;
                    packet pkt;
                    std::memcpy(&pkt.sequence_no, recv_buffer_.data(), sizeof(pkt.sequence_no));
                    std::memcpy(&pkt.packet_size, recv_buffer_.data() + sizeof(pkt.sequence_no),
                                sizeof(pkt.packet_size));

                    if (bytes_recvd >=
                        sizeof(pkt.sequence_no) + sizeof(pkt.packet_size) + pkt.packet_size) {
                        std::memcpy(pkt.data,
                                    recv_buffer_.data() + sizeof(pkt.sequence_no) +
                                        sizeof(pkt.packet_size),
                                    pkt.packet_size);
                        handle_reliable_packet(pkt);

                    } else {
                        std::cerr << "Received malformed packet" << std::endl;
                    }
                } else {
                    std::string message(recv_buffer_.data(), bytes_recvd);
                    handle_unreliable_packet(message);
                }
            }
            start_receive();
        });
}

void UDPClient::handle_reliable_packet(const packet& pkt) {
    // Print the received packet details
    std::cout << "Received packet with sequence number: " << pkt.sequence_no << std::endl;
    std::cout << "Packet size: " << pkt.packet_size << std::endl;
    std::cout << "Packet data: " << std::string(pkt.data, pkt.packet_size) << std::endl;

    // Send ACK back to the server
    send_unreliable_packet("ACK:" + std::to_string(pkt.sequence_no), server_endpoint_);
    // send_ack(pkt.sequence_no);
}

void UDPClient::send_ack(std::uint32_t sequence_number) {
    std::string ack_message = "ACK:" + std::to_string(sequence_number);
    std::cout << "Sending ACK: " << ack_message << " to " << server_endpoint_ << std::endl;
    auto buffer = std::make_shared<std::string>(ack_message);
    if (server_endpoint_.address().is_unspecified()) {
        std::cerr << "Server endpoint is unspecified" << std::endl;
        return;
    }
    socket_.async_send_to(asio::buffer(*buffer), server_endpoint_,
                          [this, buffer](std::error_code ec, std::size_t /*bytes_sent*/) {
                              if (ec) {
                                  std::cerr << "Send ACK error: " << ec.message() << std::endl;
                              }
                          });
}

void UDPClient::send_unreliable_packet(const std::string&             message,
                                       const asio::ip::udp::endpoint& server_endpoint) {
    auto packet = std::make_shared<std::string>(message);
    std::cout << "Sending unreliable packet: " << *packet << " to " << server_endpoint << std::endl;
    socket_.async_send_to(asio::buffer(*packet), server_endpoint,
                          [this, packet](std::error_code ec, std::size_t bytes_sent) {
                              if (!ec) {
                                  std::cout << " (" << bytes_sent << " bytes)" << std::endl;
                              } else {
                                  std::cerr << "Failed to send ACK: " << ec.message() << std::endl;
                              }
                          });
}

void UDPClient::handle_unreliable_packet(const std::string& message) {
    // Process the message content
    std::cout << "Processing unreliable message: " << message << std::endl;
}

extern "C" CLIENT_API UDPClient* create_udp_client(asio::io_context& io_context,
                                                   unsigned short    port) {
    return new UDPClient(io_context, port);
}