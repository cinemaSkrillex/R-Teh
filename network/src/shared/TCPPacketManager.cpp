/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** TCPPacketManager.cpp
*/

#include "../../include/shared/TCPPacketManager.hpp"

#include <TCPPacketUtils.hpp>

TCPPacketManager::TCPPacketManager(Role role)
    : _io_context(),
      _socket(std::make_shared<asio::ip::tcp::socket>(_io_context)),
      _role(role),
      _work_guard(asio::make_work_guard(_io_context)) {}

void TCPPacketManager::start_server(unsigned short port) {
    auto acceptor = std::make_shared<asio::ip::tcp::acceptor>(
        _io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port));

    accept_clients(acceptor);
    _io_thread = std::thread([this]() { _io_context.run(); });
    _io_thread.detach();
}

void TCPPacketManager::start_client(const std::string& host, short port) {
    asio::ip::tcp::resolver resolver(_io_context);
    auto                    endpoints = resolver.resolve(host, std::to_string(port));
    asio::connect(*_socket, endpoints);

    _io_thread = std::thread([this]() { _io_context.run(); });
    _io_thread.detach();
    listen_for_server_data();
}

void TCPPacketManager::accept_clients(std::shared_ptr<asio::ip::tcp::acceptor> acceptor) {
    auto new_socket = std::make_shared<asio::ip::tcp::socket>(_io_context);
    acceptor->async_accept(*new_socket, [this, acceptor, new_socket](asio::error_code ec) {
        if (!ec) {
            _client_sockets.push_back(new_socket);
            if (_new_client_callback) {
                _new_client_callback(new_socket->remote_endpoint());
            }
            listen_for_client_data(new_socket);
        } else {
            std::cerr << "Accept error: " << ec.message() << std::endl;
        }
        accept_clients(acceptor);
    });
}

void TCPPacketManager::send_message_to_client(const std::string& message) {
    for (auto& client_socket : _client_sockets) {
        if (!client_socket->is_open()) {
            std::cerr << "Client socket is not open." << std::endl;
            continue;
        }
        // auto packet = build_packet(TCPFlags::DATA, message, client_socket->remote_endpoint());
        TCPPacket packet;
        packet.sequence_nb       = 0;
        packet.start_sequence_nb = 0;
        packet.end_sequence_nb   = 0;
        packet.packet_size       = message.size();
        packet.flag              = TCPFlags::DATA;
        packet.endpoint          = client_socket->remote_endpoint();
        packet.data.assign(message.begin(), message.end());

        // auto serialized_data = std::make_shared<std::vector<char>>(packet.serialize());
        send_packet(client_socket, packet);
    }
}

void TCPPacketManager::send_message_to_client_endpoint(const std::string&             message,
                                                       const asio::ip::tcp::endpoint& endpoint) {
    for (auto& client_socket : _client_sockets) {
        if (!client_socket->is_open()) {
            std::cerr << "Client socket is not open." << std::endl;
            continue;
        }
        if (client_socket->remote_endpoint() == endpoint) {
            // auto packet = build_packet(TCPFlags::DATA, message,
            // client_socket->remote_endpoint());
            // send_packet(client_socket, packet);
            return;
        }
    }
}

void TCPPacketManager::send_packet(std::shared_ptr<asio::ip::tcp::socket> socket,
                                   const TCPPacket&                       packet) {
    if (!socket->is_open()) {
        std::cerr << "Socket is not open, cannot send packet." << std::endl;
        return;
    }

    try {
        // Serialize the packet
        const auto buffer = std::make_shared<std::vector<char>>(serialize(packet));
        asio::async_write(
            *socket, asio::buffer(*buffer),
            [this, buffer](const asio::error_code& ec, std::size_t bytes_transferred) {
                if (ec) std::cerr << "Failed to send packet: " << ec.message() << std::endl;
            });
    } catch (const std::exception& e) {
        std::cerr << "Failed to send packet: " << e.what() << std::endl;
    }
}

void TCPPacketManager::send_message_to_server(const std::string& message) {
    if (!_socket->is_open()) {
        std::cerr << "Socket is not open, cannot send message." << std::endl;
        return;
    }
    auto message_ptr = std::make_shared<std::string>(message);
    // TODO: send a message in a PACKET (BUILD PACKET TODO)
    asio::async_write(*_socket, asio::buffer(*message_ptr),
                      [message_ptr](asio::error_code ec, std::size_t /*length*/) {
                          if (ec)
                              std::cerr << "Failed to send message: " << ec.message() << std::endl;
                      });
}

void TCPPacketManager::listen_for_server_data() {
    auto self(shared_from_this());
    _socket->async_read_some(
        asio::buffer(recv_buffer_), [this, self](std::error_code ec, std::size_t bytes_recvd) {
            if (!ec && bytes_recvd > 0) {
                handle_receive(bytes_recvd);
            } else {
                if (ec) {
                    std::cerr << "Receive error: " << ec.message() << std::endl;
                } else {
                    std::cerr << "Received 0 bytes" << std::endl;
                }
            }
            listen_for_server_data();
        });
}

void TCPPacketManager::handle_receive(std::size_t bytes_recvd) {
    if (bytes_recvd == 0) {
        std::cerr << "Received 0 bytes." << std::endl;
        return;
    }
}

// TODO handle_receive with the start_sequence_nb and end_sequence_nb and file_name

void TCPPacketManager::listen_for_client_data(
    std::shared_ptr<asio::ip::tcp::socket> client_socket) {
    if (!client_socket->is_open()) {
        std::cerr << "Client socket is not open." << std::endl;
        return;
    }
    auto buffer = std::make_shared<std::array<char, 800>>();
    client_socket->async_read_some(
        asio::buffer(*buffer),
        [this, client_socket, buffer](asio::error_code ec, std::size_t length) {
            if (!ec) {
                listen_for_client_data(client_socket);
            } else {
                if (ec != asio::error::eof)
                    std::cerr << "Client read error: " << ec.message() << std::endl;
                _client_sockets.erase(
                    std::remove(_client_sockets.begin(), _client_sockets.end(), client_socket),
                    _client_sockets.end());
                if (_disconnect_client_callback) {
                    _disconnect_client_callback(client_socket->remote_endpoint());
                }
            }
        });
}

void TCPPacketManager::close() {
    if (_socket->is_open()) {
        _socket->close();
    } else {
        std::cerr << "Socket was already closed." << std::endl;
    }
    if (!_client_sockets.empty()) {
        for (auto& client_socket : _client_sockets) {
            if (client_socket->is_open()) {
                client_socket->close();
            }
        }
        _client_sockets.clear();
    }
    _work_guard.reset();
    _io_context.stop();
    if (_io_thread.joinable()) {
        _io_thread.join();
    }
}