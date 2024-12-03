/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** TCPPacketManager.cpp
*/

#include "../../include/shared/TCPPacketManager.hpp"

TCPPacketManager::TCPPacketManager(Role role)
    : _io_context(),
      _socket(std::make_shared<asio::ip::tcp::socket>(_io_context)),
      _role(role),
      _work_guard(asio::make_work_guard(_io_context)) {}

void TCPPacketManager::start_server(unsigned short port) {
    auto acceptor = std::make_shared<asio::ip::tcp::acceptor>(
        _io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port));
    std::cout << "Server listening on port " << port << std::endl;

    accept_clients(acceptor);
    _io_thread = std::thread([this]() { _io_context.run(); });
    _io_thread.detach();
}

void TCPPacketManager::start_client(const std::string& host, short port) {
    asio::ip::tcp::resolver resolver(_io_context);
    auto                    endpoints = resolver.resolve(host, std::to_string(port));
    asio::connect(*_socket, endpoints);

    std::cout << "Connected to server at " << host << ":" << port << std::endl;

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

TCPPacket TCPPacketManager::build_packet(TCPFlags flag, const std::string& message,
                                         const asio::ip::tcp::endpoint& endpoint) {
    TCPPacket packet;
    packet.flag = flag;
    packet.data.assign(message.begin(), message.end());
    packet.endpoint    = endpoint;
    packet.packet_size = sizeof(packet.flag) + sizeof(size_t) + packet.data.size() +
                         sizeof(size_t) + endpoint.address().to_string().size() + sizeof(uint16_t);
    return packet;
}

// void TCPPacketManager::send_message_to_client(const std::string& message) {
//     for (auto& client_socket : _client_sockets) {
//         if (!client_socket->is_open()) {
//             std::cerr << "Client socket is not open." << std::endl;
//             continue;
//         }
//         auto message_ptr = std::make_shared<std::string>(message);
//         asio::async_write(
//             *client_socket, asio::buffer(*message_ptr),
//             [client_socket, message_ptr](asio::error_code ec, std::size_t /*length*/) {
//                 if (!ec) {
//                     std::cout << "Message sent successfully." << std::endl;
//                 } else {
//                     std::cerr << "Failed to send message: " << ec.message() << std::endl;
//                 }
//             });
//     }
// }

void TCPPacketManager::send_message_to_client(const std::string& message) {
    for (auto& client_socket : _client_sockets) {
        if (!client_socket->is_open()) {
            std::cerr << "Client socket is not open." << std::endl;
            continue;
        }
        auto packet = build_packet(TCPFlags::DATA, message, client_socket->remote_endpoint());
        auto serialized_data = std::make_shared<std::vector<char>>(packet.serialize());
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
            auto packet = build_packet(TCPFlags::DATA, message, client_socket->remote_endpoint());
            auto serialized_data = std::make_shared<std::vector<char>>(packet.serialize());
            send_packet(client_socket, packet);
            // auto message_ptr = std::make_shared<std::string>(message);
            // asio::async_write(
            //     *client_socket, asio::buffer(*message_ptr),
            //     [client_socket, message_ptr](asio::error_code ec, std::size_t /*length*/) {
            //         if (!ec) {
            //             std::cout << "Message sent successfully." << std::endl;
            //         } else {
            //             std::cerr << "Failed to send message: " << ec.message() << std::endl;
            //         }
            //     });
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
    auto serialized_data = std::make_shared<std::vector<char>>(packet.serialize());
    asio::async_write(*socket, asio::buffer(*serialized_data),
                      [serialized_data](asio::error_code ec, std::size_t /*length*/) {
                          if (ec) {
                              std::cerr << "Failed to send packet: " << ec.message() << std::endl;
                          } else {
                              std::cout << "Packet sent successfully." << std::endl;
                          }
                      });
}

void TCPPacketManager::send_message_to_server(const std::string& message) {
    if (!_socket->is_open()) {
        std::cerr << "Socket is not open, cannot send message." << std::endl;
        return;
    }
    std::cout << "socket endpoint: " << _socket->remote_endpoint() << std::endl;
    auto message_ptr = std::make_shared<std::string>(message);
    asio::async_write(*_socket, asio::buffer(*message_ptr),
                      [message_ptr](asio::error_code ec, std::size_t /*length*/) {
                          if (!ec) {
                              std::cout << "Message sent successfully." << std::endl;
                          } else {
                              std::cerr << "Failed to send message: " << ec.message() << std::endl;
                          }
                      });
}

// void TCPPacketManager::listen_for_server_data() {
//     auto self(shared_from_this());
//     _socket->async_read_some(
//         asio::buffer(data_, max_length), [this](asio::error_code ec, std::size_t length) {
//             if (!ec) {
//                 std::cout << "Received: " << std::string(data_, length) << std::endl;
//                 listen_for_server_data();

//             } else {
//                 std::cerr << "Listen_for_server_data Error: " << ec.message() << std::endl;
//             }
//         });
// }

void TCPPacketManager::listen_for_server_data() {
    auto self(shared_from_this());
    _socket->async_receive(asio::buffer(recv_buffer_), [this, self](std::error_code ec,
                                                                    std::size_t     bytes_recvd) {
        if (!ec && bytes_recvd > 0) {
            std::cout << "Received: " << std::string(recv_buffer_.data(), bytes_recvd) << std::endl;
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
    try {
        std::vector<char> buffer(recv_buffer_.begin(), recv_buffer_.begin() + bytes_recvd);
        TCPPacket         packet = TCPPacket::deserialize(buffer);
        std::cout << "Received packet: " << packet << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Failed to deserialize packet: " << e.what() << std::endl;
    }
}

void TCPPacketManager::listen_for_client_data(
    std::shared_ptr<asio::ip::tcp::socket> client_socket) {
    if (!client_socket->is_open()) {
        std::cerr << "Client socket is not open." << std::endl;
        return;
    }
    auto buffer = std::make_shared<std::array<char, 1024>>();
    client_socket->async_read_some(
        asio::buffer(*buffer),
        [this, client_socket, buffer](asio::error_code ec, std::size_t length) {
            if (!ec) {
                std::cout << "Received from client: " << std::string(buffer->data(), length)
                          << std::endl;

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
        std::cout << "Socket closed." << std::endl;
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

void TCPPacketManager::ensure_directory_exists(const std::string& directory_path) {
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