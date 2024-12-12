/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** TCPPacketManager.cpp
*/

#include "TCPPacketManager.hpp"
#include <iostream>
#include <fstream>

TCPPacketManager::TCPPacketManager(Role role)
    : _role(role),
      _socket(std::make_shared<asio::ip::tcp::socket>(_io_context)),
      _work_guard(asio::make_work_guard(_io_context)) {}

void TCPPacketManager::start_server(unsigned short port) {
    auto acceptor = std::make_shared<asio::ip::tcp::acceptor>(
        _io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port));
    accept_clients(acceptor);
    _io_thread = std::thread([this]() { _io_context.run(); });
}

void TCPPacketManager::start_client(const std::string& host, unsigned short port) {
    asio::ip::tcp::resolver resolver(_io_context);
    auto endpoints = resolver.resolve(host, std::to_string(port));
    asio::connect(*_socket, endpoints);
    std::cout << "Connected to server at " << host << ":" << port << std::endl;
    listen_for_server_data();

    _io_thread = std::thread([this]() { _io_context.run(); });
}


void TCPPacketManager::accept_clients(std::shared_ptr<asio::ip::tcp::acceptor> acceptor) {
    auto new_socket = std::make_shared<asio::ip::tcp::socket>(_io_context);
    acceptor->async_accept(*new_socket, [this, acceptor, new_socket](asio::error_code ec) {
        if (!ec) {
            _client_sockets.push_back(new_socket);
            if (_new_client_callback) {
                _new_client_callback(new_socket->remote_endpoint());
            }
        }
        accept_clients(acceptor);
    });
}

void TCPPacketManager::send_message_to_server(const std::string& message) {
    asio::async_write(*_socket, asio::buffer(message),
                      [](std::error_code ec, std::size_t) {
                          if (ec) {
                              std::cerr << "Error sending message: " << ec.message() << std::endl;
                          }
                      });
}

void TCPPacketManager::send_message_to_client_endpoint(const std::string& message,
                                                       const asio::ip::tcp::endpoint& endpoint) {
    for (auto& client_socket : _client_sockets) {
        if (!client_socket->is_open()) {
            std::cerr << "Socket is not open. Cannot send message to " << endpoint << std::endl;
            continue;
        }

        if (client_socket->remote_endpoint() == endpoint) {
            std::string formatted_message = "MSG:" + message + "\n";
            std::cout << "Sending message to client: " << message << std::endl;
            try {
                asio::write(*client_socket, asio::buffer(formatted_message));
                std::cout << "Message sent successfully." << std::endl;
            } catch (const std::system_error& e) {
                std::cerr << "Error sending message: " << e.what() << std::endl;
            }
            break;
        }
    }
}


void TCPPacketManager::receive_file_data(std::shared_ptr<std::ofstream> file,
                                         std::size_t expected_size) {
    std::vector<char> buffer(4096);
    std::size_t total_received = 0;

    while (total_received < expected_size) {
        std::size_t bytes_to_read = std::min(static_cast<std::size_t>(4096), expected_size - total_received);
        asio::error_code ec;
        std::size_t bytes_read = _socket->read_some(asio::buffer(buffer.data(), bytes_to_read), ec);

        if (ec) {
            std::cerr << "Error receiving file data: " << ec.message() << std::endl;
            return;
        }
        file->write(buffer.data(), bytes_read);
        total_received += bytes_read;
        // std::cout << "Received chunk: " << bytes_read << " bytes (Total: " << total_received
        //           << "/" << expected_size << ")" << std::endl;
    }

    file->close();
    std::cout << "File received and saved successfully." << std::endl;
}

void TCPPacketManager::listen_for_server_data() {
    std::vector<char> buffer(4096);
    asio::error_code ec;

    while (true) {
        std::size_t length = _socket->read_some(asio::buffer(buffer), ec);
        if (ec) {
            if (ec == asio::error::eof) {
                std::cout << "Connection closed by server." << std::endl;
                break;
            } else {
                std::cerr << "Error reading from server: " << ec.message() << std::endl;
                break;
            }
        }
        std::string data(buffer.data(), length);
        while (!data.empty()) {
            if (data.rfind("MSG:", 0) == 0) {
                size_t end_pos = data.find('\n');
                if (end_pos == std::string::npos) {
                    std::cerr << "Malformed message received." << std::endl;
                    break;
                }
                std::string message = data.substr(4, end_pos - 4);
                std::cout << "Message received: " << message << std::endl;
                data = data.substr(end_pos + 1);
            } else if (data.rfind("FILE:", 0) == 0) {
                size_t name_end = data.find(':', 5);
                size_t size_end = data.find('\n', name_end + 1);
                if (name_end == std::string::npos || size_end == std::string::npos) {
                    std::cerr << "Malformed file header received." << std::endl;
                    break;
                }
                std::string file_name = data.substr(5, name_end - 5);
                std::size_t file_size = std::stoul(data.substr(name_end + 1, size_end - name_end - 1));
                std::cout << "Receiving file: " << file_name << " (" << file_size << " bytes)" << std::endl;
                data = data.substr(size_end + 1);

                std::filesystem::path path("test");
                if (!std::filesystem::exists(path)) {
                    try {
                        std::filesystem::create_directory(path);
                        std::cout << "Created directory: " << path << std::endl;
                    } catch (const std::filesystem::filesystem_error& e) {
                        std::cerr << "Error creating directory: " << e.what() << std::endl;
                        break;
                    }
                }

                size_t file_data_in_buffer = std::min(file_size, data.size());
                std::ofstream file("test/" + file_name, std::ios::binary);

                if (!file.is_open()) {
                    std::cerr << "Failed to open file for writing: " << file_name << std::endl;
                    break;
                }
                file.write(data.data(), file_data_in_buffer);
                file_size -= file_data_in_buffer;
                data = data.substr(file_data_in_buffer);
                while (file_size > 0) {
                    std::vector<char> file_buffer(4096);
                    std::size_t bytes_to_read = std::min(static_cast<std::size_t>(4096), file_size);
                    std::size_t bytes_read = _socket->read_some(asio::buffer(file_buffer.data(), bytes_to_read), ec);
                    if (ec) {
                        std::cerr << "Error receiving file data: " << ec.message() << std::endl;
                        return;
                    }
                    file.write(file_buffer.data(), bytes_read);
                    file_size -= bytes_read;
                    // std::cout << "Received chunk: " << bytes_read << " bytes (Remaining: " << file_size << ")" << std::endl;
                }
                file.close();
                std::cout << "File received and saved successfully." << std::endl;
            } else {
                std::cerr << "Unknown data format received." << std::endl;
                break;
            }
        }
    }
}


void TCPPacketManager::send_file_to_client(const std::string& file_path,
                                           const asio::ip::tcp::endpoint& endpoint) {
    auto file = std::make_shared<std::ifstream>(file_path, std::ios::binary);
    if (!file->is_open()) {
        std::cerr << "Error opening file: " << file_path << std::endl;
        return;
    }
    file->seekg(0, std::ios::end);
    std::size_t file_size = file->tellg();
    file->seekg(0, std::ios::beg);
    std::string file_name = file_path.substr(file_path.find_last_of("/\\") + 1);
    std::string header = "FILE:" + file_name + ":" + std::to_string(file_size) + "\n";
    auto header_buffer = std::make_shared<std::vector<char>>(header.begin(), header.end());
    std::cout << "Preparing to send file: " << file_name << " (" << file_size << " bytes)" << std::endl;
    for (auto& client_socket : _client_sockets) {
        if (client_socket->remote_endpoint() == endpoint) {
            if (!client_socket->is_open()) {
                std::cerr << "Socket is not open for endpoint: " << endpoint << std::endl;
                return;
            }
            asio::write(*client_socket, asio::buffer(*header_buffer));
            auto chunk_buffer = std::make_shared<std::vector<char>>(4096);
            while (!file->eof()) {
                file->read(chunk_buffer->data(), chunk_buffer->size());
                std::streamsize bytes_read = file->gcount();
                if (!client_socket->is_open()) { 
                    std::cerr << "Socket closed during file transmission for endpoint: " << endpoint
                              << std::endl;
                    return;
                }
                asio::write(*client_socket, asio::buffer(chunk_buffer->data(), bytes_read));
                // std::cout << "Sent chunk: " << bytes_read << " bytes" << std::endl;
            }
            std::cout << "File sent successfully to " << endpoint << std::endl;
            break;
        }
    }
}


void TCPPacketManager::close() {
    _io_context.stop();
    if (_io_thread.joinable()) {
        _io_thread.join();
    }
}
