/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** TCPServer.cpp
*/

#include "../../include/Server/TCPServer.hpp"

TCPServer::TCPServer(asio::io_context& io_context, unsigned short port)
    : acceptor_(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) {
    start_accept();
}

void TCPServer::start_accept() {
    auto socket = std::make_shared<asio::ip::tcp::socket>(acceptor_.get_executor());
    acceptor_.async_accept(
        *socket, [this, socket](const asio::error_code& error) { handle_accept(socket, error); });
}

void TCPServer::handle_accept(std::shared_ptr<asio::ip::tcp::socket> socket,
                              const asio::error_code&                error) {
    if (!error) {
        auto buffer = std::make_shared<std::array<char, 1024>>();
        socket->async_read_some(
            asio::buffer(*buffer),
            [this, socket, buffer](const asio::error_code& error, std::size_t bytes_transferred) {
                handle_read(socket, buffer, error, bytes_transferred);
            });
    }
    start_accept();
}

void TCPServer::handle_read(std::shared_ptr<asio::ip::tcp::socket>  socket,
                            std::shared_ptr<std::array<char, 1024>> buffer,
                            const asio::error_code& error, std::size_t bytes_transferred) {
    if (!error) {
        std::string message(buffer->data(), bytes_transferred);
        std::cout << "Received: " << message << std::endl;

        if (message == "ping") {
            auto response = std::make_shared<std::string>("pong");
            asio::async_write(*socket, asio::buffer(*response),
                              [this, socket, response](const asio::error_code& error,
                                                       std::size_t             bytes_transferred) {
                                  handle_write(socket, error, bytes_transferred);
                              });
        } else {
            auto new_buffer = std::make_shared<std::array<char, 1024>>();
            socket->async_read_some(asio::buffer(*new_buffer),
                                    [this, socket, new_buffer](const asio::error_code& error,
                                                               std::size_t bytes_transferred) {
                                        handle_read(socket, new_buffer, error, bytes_transferred);
                                    });
        }
    }
}

void TCPServer::handle_write(std::shared_ptr<asio::ip::tcp::socket> socket,
                             const asio::error_code& error, std::size_t bytes_transferred) {
    if (!error) {
        auto buffer = std::make_shared<std::array<char, 1024>>();
        socket->async_read_some(
            asio::buffer(*buffer),
            [this, socket, buffer](const asio::error_code& error, std::size_t bytes_transferred) {
                handle_read(socket, buffer, error, bytes_transferred);
            });
    }
}