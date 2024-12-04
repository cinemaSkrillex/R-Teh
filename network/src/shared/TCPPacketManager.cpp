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

void TCPPacketManager::send_message_to_client(const std::string& message) {
    for (auto& client_socket : _client_sockets) {
        if (!client_socket->is_open()) {
            std::cerr << "Client socket is not open." << std::endl;
            continue;
        }
    }
}

std::shared_ptr<std::vector<char>> TCPPacketManager::serialize_string(const std::string& message,
                                                                      TCPFlags           flag) {
    // int message_size = message.size();
    // // std::vector<char> buffer(sizeof(int) + message_size);
    // std::vector<char> buffer(sizeof(TCPFlags) + sizeof(int) + message_size);

    // // Copy the flag to the beginning of the buffer
    // std::memcpy(buffer.data(), &flag, sizeof(TCPFlags));

    // // Copy the size of the message after the flag
    // std::memcpy(buffer.data() + sizeof(TCPFlags), &message_size, sizeof(int));

    // // Copy the message content after the size
    // std::memcpy(buffer.data() + sizeof(TCPFlags) + sizeof(int), message.data(), message_size);

    // return buffer;

    int message_size = message.size();
    int total_size   = sizeof(TCPFlags) + message_size;
    std::cout << "totalsize" << total_size << std::endl;
    auto buffer =
        std::make_shared<std::vector<char>>(sizeof(int) + sizeof(TCPFlags) + message_size);

    // Copy the total size to the beginning of the buffer
    std::memcpy(buffer->data(), &total_size, sizeof(int));

    // Copy the flag after the total size
    std::memcpy(buffer->data() + sizeof(int), &flag, sizeof(TCPFlags));

    // Copy the message content after the flag
    std::memcpy(buffer->data() + sizeof(int) + sizeof(TCPFlags), message.data(), message_size);

    return buffer;
}

void TCPPacketManager::send_message(std::shared_ptr<std::vector<char>>     serialized_message,
                                    std::shared_ptr<asio::ip::tcp::socket> receiver_socket) {
    asio::async_write(*receiver_socket, asio::buffer(*serialized_message),
                      [this](std::error_code ec, std::size_t bytes_sent) {
                          if (!ec) {
                              std::cout << "Message sent successfully." << std::endl;
                          } else {
                              std::cerr << "Failed to send message: " << ec.message() << std::endl;
                          }
                      });
}

void TCPPacketManager::send_message_to_client_endpoint(const std::string&             message,
                                                       const asio::ip::tcp::endpoint& endpoint) {
    std::shared_ptr<asio::ip::tcp::socket> receiver_socket;
    for (auto& client_socket : _client_sockets) {
        if (!client_socket->is_open()) {
            std::cerr << "Client socket is not open." << std::endl;
            continue;
        }
        if (client_socket->remote_endpoint() == endpoint) {
            receiver_socket = client_socket;
            break;
        }
    }
    auto serialized_message = serialize_string(message, TCPFlags::MESSAGE);
    send_message(serialized_message, receiver_socket);
}

void TCPPacketManager::send_message_to_server(const std::string& message) {
    if (!_socket->is_open()) {
        std::cerr << "Socket is not open, cannot send message." << std::endl;
        return;
    }
    std::cout << "socket endpoint: " << _socket->remote_endpoint() << std::endl;
    auto message_ptr = std::make_shared<std::string>(message);
    // TODO: send a message in a PACKET (BUILD PACKET TODO)
    asio::async_write(*_socket, asio::buffer(*message_ptr),
                      [message_ptr](asio::error_code ec, std::size_t /*length*/) {
                          if (!ec) {
                              std::cout << "Message sent successfully." << std::endl;
                          } else {
                              std::cerr << "Failed to send message: " << ec.message() << std::endl;
                          }
                      });
}

void TCPPacketManager::receive_file_data(std::shared_ptr<std::vector<char>> file_buffer,
                                         size_t total_size, size_t bytes_received) {
    auto self(shared_from_this());

    if (bytes_received >= total_size) {
        // All data has been received
        std::ofstream file("received_file.pdf", std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Failed to open file for writing." << std::endl;
            return;
        }
        file.write(file_buffer->data(), total_size);
        file.close();
        std::cout << "File written successfully. Total bytes: " << total_size << std::endl;

        // callback
        if (_receive_file_callback) {
            _receive_file_callback();
        }

        // Listen for more server data
        listen_for_server_data();
        return;
    }

    // Calculate remaining bytes to read
    size_t remaining_size = total_size - bytes_received;
    size_t chunk_size = std::min(remaining_size, static_cast<size_t>(64 * 1024));  // 64 KB chunks

    _socket->async_read_some(asio::buffer(file_buffer->data() + bytes_received, chunk_size),
                             [this, self, file_buffer, total_size, bytes_received](
                                 asio::error_code ec, std::size_t length) {
                                 if (!ec) {
                                     size_t new_bytes_received = bytes_received + length;
                                     std::cout << "Received " << new_bytes_received << " of "
                                               << total_size << " bytes." << std::endl;

                                     // Continue receiving the remaining data
                                     receive_file_data(file_buffer, total_size, new_bytes_received);
                                 } else {
                                     std::cerr << "Error while reading file data: " << ec.message()
                                               << std::endl;
                                 }
                             });
}

// void TCPPacketManager::listen_for_server_data() {
//     auto self(shared_from_this());
//     if (!_socket->is_open()) {
//         std::cerr << "Socket is not open." << std::endl;
//         return;
//     }
//     // read an int describing the size of the file
//     auto buffer = std::make_shared<std::array<char, sizeof(int)>>();
//     _socket->async_read_some(
//         asio::buffer(*buffer), [this, self, buffer](asio::error_code ec, std::size_t length) {
//             if (!ec) {
//                 int size;
//                 std::memcpy(&size, buffer->data(), sizeof(int));
//                 std::cout << "Received " << length << " bytes. File size: " << size << std::endl;
//                 // read the file
//                 // Step 2: Start receiving the file data
//                 auto file_buffer = std::make_shared<std::vector<char>>(size);
//                 receive_file_data(file_buffer, size, 0);
//             } else {
//                 if (ec != asio::error::eof) {
//                     std::cerr << "Server read error: " << ec.message() << std::endl;
//                 }
//             }
//         });
// }

void TCPPacketManager::listen_for_server_data() {
    auto self(shared_from_this());
    if (!_socket->is_open()) {
        std::cerr << "Socket is not open." << std::endl;
        return;
    }

    // Step 1: Read the total size
    auto size_buffer = std::make_shared<std::array<char, sizeof(int)>>();
    _socket->async_read_some(asio::buffer(*size_buffer), [this, self, size_buffer](
                                                             asio::error_code ec,
                                                             std::size_t      length) {
        if (!ec) {
            int total_size;
            std::memcpy(&total_size, size_buffer->data(), sizeof(int));
            std::cout << "Received " << length << " bytes. Total size: " << total_size << std::endl;

            // Step 2: Read the flag
            auto flag_buffer = std::make_shared<std::array<char, sizeof(TCPFlags)>>();
            _socket->async_read_some(asio::buffer(*flag_buffer), [this, self, flag_buffer,
                                                                  total_size](asio::error_code ec,
                                                                              std::size_t length) {
                if (!ec) {
                    TCPFlags flag;
                    std::memcpy(&flag, flag_buffer->data(), sizeof(TCPFlags));
                    // std::cout << "Received flag: " << flag << std::endl;

                    // Step 3: Read the data based on the flag
                    int data_size = total_size - sizeof(TCPFlags);
                    if (flag == TCPFlags::MESSAGE) {
                        // Read the message content
                        auto message_buffer = std::make_shared<std::vector<char>>(data_size);
                        _socket->async_read_some(
                            asio::buffer(*message_buffer),
                            [this, self, message_buffer](asio::error_code ec, std::size_t length) {
                                if (!ec) {
                                    std::string message(message_buffer->begin(),
                                                        message_buffer->end());
                                    std::cout << "Received message: " << message << std::endl;
                                    listen_for_server_data();
                                } else {
                                    std::cerr << "Message read error: " << ec.message()
                                              << std::endl;
                                }
                            });
                    } else if (flag == TCPFlags::IMAGE) {
                        // Read the file data
                        // auto file_buffer = std::make_shared<std::vector<char>>(data_size);
                        // receive_file_data(file_buffer, data_size, 0);
                        auto        file_buffer = std::make_shared<std::vector<char>>(data_size);
                        std::thread file_thread([this, file_buffer, data_size]() {
                            receive_file_data(file_buffer, data_size, 0);
                        });

                        // Detach the thread so it can run independently
                        file_thread.detach();

                        // Continue listening for other server data
                        listen_for_server_data();
                    }
                } else {
                    if (ec != asio::error::eof) {
                        std::cerr << "Flag read error: " << ec.message() << std::endl;
                        listen_for_server_data();
                    }
                }
            });
        } else {
            if (ec != asio::error::eof) {
                std::cerr << "Size read error: " << ec.message() << std::endl;
            }
        }
    });
}

void TCPPacketManager::handle_receive(std::size_t bytes_recvd) {}

// TODO handle_receive with the start_sequence_nb and end_sequence_nb and file_name

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

void TCPPacketManager::send_file_to_client(const std::string&             file_path,
                                           const asio::ip::tcp::endpoint& endpoint) {
    std::ifstream file(file_path, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << file_path << std::endl;
        return;
    }
    std::cout << "Opened file: " << file_path << std::endl;

    std::shared_ptr<asio::ip::tcp::socket> current_socket;

    for (auto& client_socket : _client_sockets) {
        if (!client_socket->is_open()) {
            std::cerr << "Client socket is not open." << std::endl;
            continue;
        }
        if (client_socket->remote_endpoint() == endpoint) {
            current_socket = client_socket;
            break;
        }
    }

    if (!current_socket) {
        std::cerr << "Client socket not found." << std::endl;
        return;
    }
    std::cout << "currentsocket endpoint: " << current_socket->remote_endpoint() << std::endl;

    file.seekg(0, std::ios::end);
    std::streamsize file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    if (file_size == 0) {
        std::cerr << "File is empty." << std::endl;
        return;
    }

    auto file_data = std::make_shared<std::vector<char>>(file_size);
    if (!file.read(file_data->data(), file_size)) {
        std::cerr << "Failed to read file." << std::endl;
        return;
    }

    // add an int describing the size of the file at the beginning of the file_data
    // int  size           = file_data->size();
    // auto data_with_size = std::make_shared<std::vector<char>>(sizeof(int) + file_data->size());
    // std::memcpy(data_with_size->data(), &size, sizeof(int));
    // std::memcpy(data_with_size->data() + sizeof(int), file_data->data(), file_data->size());

    // std::cout << data_with_size->size() << " bytes prepared for sending." << std::endl;
    std::shared_ptr<std::vector<char>> serialized_data =
        serialize_string(std::string(file_data->begin(), file_data->end()), TCPFlags::IMAGE);
    std::cout << serialized_data->size() << " bytes prepared for sending." << std::endl;
    if (!current_socket || !current_socket->is_open()) {
        std::cerr << "Socket is not valid or not open." << std::endl;
        return;
    }

    asio::async_write(*current_socket, asio::buffer(*serialized_data),
                      [serialized_data](const asio::error_code& ec, std::size_t bytes_transferred) {
                          if (!ec) {
                              std::cout << "Successfully sent " << bytes_transferred << " bytes."
                                        << std::endl;
                          } else {
                              std::cerr << "Error during async_write: " << ec.message()
                                        << "bytes transferred; " << bytes_transferred << std::endl;
                          }
                      });
    file.close();
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