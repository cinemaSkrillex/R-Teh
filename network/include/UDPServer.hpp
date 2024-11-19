/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** UDPServer.hpp
*/

#ifndef UDPSERVER_HPP
#define UDPSERVER_HPP

#include <asio.hpp>
#include <iostream>
#include <array>

class UDPServer {
  public:
    UDPServer(asio::io_context& io_context, unsigned short port)
        : socket_(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)) {
        start_receive();
    }

  private:
    void start_receive() {
        socket_.async_receive_from(asio::buffer(recv_buffer_), remote_endpoint_,
                                   [this](std::error_code ec, std::size_t bytes_recvd) {
                                       if (!ec && bytes_recvd > 0) {
                                           handle_receive(bytes_recvd);
                                       } else {
                                           start_receive();
                                       }
                                   });
    }

    void handle_receive(std::size_t bytes_recvd) {
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

    asio::ip::udp::socket   socket_;
    asio::ip::udp::endpoint remote_endpoint_;
    std::array<char, 1024>  recv_buffer_;
};

#endif // UDPSERVER_HPP