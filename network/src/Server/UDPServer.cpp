/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** UDPServer.cpp
*/

#include "../../include/Server/UDPServer.hpp"

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
    std::string message(recv_buffer_.data(), bytes_recvd);
    std::cout << "Received: " << message << std::endl;

    if (message == "ping") {
        std::string response = "pong";
        socket_.async_send_to(
            asio::buffer(response), remote_endpoint_,
            [this](std::error_code /*ec*/, std::size_t /*bytes_sent*/) { start_receive(); });
    } else {
        start_receive();
    }
}

extern "C" SERVER_API UDPServer* create_udp_server(asio::io_context& io_context,
                                                   unsigned short    port) {
    return new UDPServer(io_context, port);
}