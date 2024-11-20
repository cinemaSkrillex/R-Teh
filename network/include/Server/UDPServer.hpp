/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** UDPServer.hpp
*/

#ifndef UDPSERVER_HPP
#define UDPSERVER_HPP

#include "../Export.hpp"
#include <asio.hpp>
#include <iostream>
#include <array>

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