/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** UDPServer.hpp
*/

#ifndef UDPSERVER_HPP
#define UDPSERVER_HPP

#if defined(_WIN32) || defined(_WIN64)
#define _WIN32_WINNT 0x0A00
#endif

#include "../Export.hpp"
#include <asio.hpp>
#include <iostream>
#include <array>

// SERVER_API is a macro for the visibility of the class UDPServer,
// its for the shared library

class SERVER_API UDPServer {
  public:
    UDPServer(asio::io_context& io_context, unsigned short port);

  private:
    void                    start_receive();
    void                    handle_receive(std::size_t bytes_recvd);
    asio::ip::udp::socket   socket_;
    asio::ip::udp::endpoint remote_endpoint_;
    std::array<char, 1024>  recv_buffer_;
};

#endif // UDPSERVER_HPP