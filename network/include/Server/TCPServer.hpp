/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** TCPServer.hpp
*/

#ifndef TCPSERVER_HPP
#define TCPSERVER_HPP

#include "../Export.hpp"
#include <asio.hpp>
#include <iostream>
#include <array>
#include <memory>

// SERVER_API is a macro for the visibility of the class UDPServer,
// its for the shared library

class SERVER_API TCPServer {
  public:
    TCPServer(asio::io_context& io_context, unsigned short port);

  private:
    void start_accept();
    void handle_accept(std::shared_ptr<asio::ip::tcp::socket> socket,
                       const asio::error_code&                error);
    void handle_read(std::shared_ptr<asio::ip::tcp::socket>  socket,
                     std::shared_ptr<std::array<char, 1024>> buffer, const asio::error_code& error,
                     std::size_t bytes_transferred);
    void handle_write(std::shared_ptr<asio::ip::tcp::socket> socket, const asio::error_code& error,
                      std::size_t bytes_transferred);

    asio::ip::tcp::acceptor acceptor_;
};

#endif // TCPSERVER_HPP