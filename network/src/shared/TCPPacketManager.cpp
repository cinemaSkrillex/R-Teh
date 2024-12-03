/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** TCPPacketManager.cpp
*/

#include "../../include/shared/TCPPacketManager.hpp"

void ensure_directory_exists(const std::string& directory_path) {
    namespace fs = std::filesystem;

    fs::path dir(directory_path);

    if (!fs::exists(dir)) {
        std::cout << "Directory does not exist. Creating: " << directory_path << std::endl;
        if (!fs::create_directories(dir)) {
            std::cerr << "Failed to create directory: " << directory_path << std::endl;
            throw std::runtime_error("Failed to create directory.");
        }
    }
}

TCPPacketManager::TCPPacketManager(Role role) : _io_context(), _socket(_io_context), _role(role) {}

void TCPPacketManager::start_server(unsigned short port) {
    auto acceptor = std::make_shared<asio::ip::tcp::acceptor>(
        _io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port));
    std::cout << "Server listening on port " << port << std::endl;

    accept_clients(acceptor);

    _io_context.run();
}

void TCPPacketManager::accept_clients(std::shared_ptr<asio::ip::tcp::acceptor> acceptor) {
    auto new_socket = std::make_shared<asio::ip::tcp::socket>(_io_context);
    acceptor->async_accept(*new_socket, [this, acceptor, new_socket](asio::error_code ec) {
        if (!ec) {
            auto message = std::make_shared<std::string>("Hello from server!");
            asio::async_write(*new_socket, asio::buffer(*message),
                              [new_socket, message](asio::error_code ec, std::size_t /*length*/) {
                                  if (!ec) {
                                  } else {
                                      std::cerr << "Failed to send message: " << ec.message()
                                                << std::endl;
                                  }
                              });
            listen_for_client_data(new_socket);
        } else {
            std::cerr << "Accept error: " << ec.message() << std::endl;
        }
        accept_clients(acceptor);
    });
}

void TCPPacketManager::listen_for_client_data(
    std::shared_ptr<asio::ip::tcp::socket> client_socket) {
    auto buffer = std::make_shared<std::array<char, 1024>>();
    client_socket->async_read_some(
        asio::buffer(*buffer),
        [this, client_socket, buffer](asio::error_code ec, std::size_t length) {
            if (!ec) {
                std::cout << "Received from client: " << std::string(buffer->data(), length)
                          << std::endl;
                // send_file("epitech.pdf", client_socket);

                listen_for_client_data(client_socket);
            } else if (ec != asio::error::eof) {
                std::cerr << "Client read error: " << ec.message() << std::endl;
            } else {
                std::cout << "Client disconnected." << std::endl;
            }
        });
}

void TCPPacketManager::start_client(const std::string& host, short port) {
    asio::ip::tcp::resolver resolver(_io_context);
    auto                    endpoints = resolver.resolve(host, std::to_string(port));
    asio::connect(_socket, endpoints);

    std::cout << "Connected to server at " << host << ":" << port << std::endl;

    send_message("Hello from client!");

    listen_for_server_data();
}

void TCPPacketManager::send_message(const std::string& message) {
    asio::write(_socket, asio::buffer(message));
}

void TCPPacketManager::listen_for_server_data() {
    _socket.async_read_some(
        asio::buffer(data_, max_length), [this](asio::error_code ec, std::size_t length) {
            if (!ec) {
                std::cout << "Received: " << std::string(data_, length) << std::endl;
                auto socket_ptr = std::make_shared<asio::ip::tcp::socket>(std::move(_socket));
                // receive_file(socket_ptr, "./received_files");

                listen_for_server_data();
            } else {
                std::cerr << "Error: " << ec.message() << std::endl;
            }
        });
    _io_context.run();
}

void TCPPacketManager::send_packet(std::shared_ptr<asio::ip::tcp::socket> socket,
                                   const TCPPacket&                       packet) {
    auto serialized_data = std::make_shared<std::vector<char>>(packet.serialize());
    asio::async_write(*socket, asio::buffer(*serialized_data),
                      [serialized_data](asio::error_code ec, std::size_t /*length*/) {
                          if (ec) {
                              std::cerr << "Failed to send packet: " << ec.message() << std::endl;
                          }
                      });
}

void TCPPacketManager::close() {
    if (_socket.is_open()) {
        _socket.close();
        std::cout << "Socket closed." << std::endl;
    } else {
        std::cerr << "Socket was already closed." << std::endl;
    }
    _io_context.stop();
}