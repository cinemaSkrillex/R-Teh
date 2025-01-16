/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** TCPPacketManager.cpp
*/

#include "TCPPacketManager.hpp"

#include <fstream>
#include <iostream>

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
    auto                    endpoints = resolver.resolve(host, std::to_string(port));
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
    asio::async_write(*_socket, asio::buffer(message), [](std::error_code ec, std::size_t) {
        if (ec) {
            std::cerr << "Error sending message: " << ec.message() << std::endl;
        }
    });
}

void TCPPacketManager::send_message_to_client_endpoint(const std::string&             message,
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

void TCPPacketManager::handle_file_reception(std::string&       data,
                                             const std::string& current_directory) {
    namespace fs = std::filesystem;

    size_t name_end = data.find(':', 5);
    size_t size_end = data.find('\n', name_end + 1);

    if (name_end == std::string::npos || size_end == std::string::npos) {
        std::cerr << "Malformed file header received." << std::endl;
        return;
    }

    std::string file_name = data.substr(5, name_end - 5);
    std::size_t file_size = std::stoul(data.substr(name_end + 1, size_end - name_end - 1));
    std::cout << "Receiving file: " << file_name << " (" << file_size << " bytes)" << std::endl;
    std::string directory = current_directory.empty() ? "FILE_SHARING" : current_directory;

    if (!fs::exists(directory)) {
        try {
            fs::create_directory(directory);
            std::cout << "Directory created: " << directory << std::endl;
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Failed to create directory '" << directory << "': " << e.what()
                      << std::endl;
            return;
        }
    }

    std::string file_path = directory + "/" + file_name;

    std::ofstream file(file_path, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << file_path << std::endl;
        return;
    }
    std::string file_data           = data.substr(size_end + 1);
    size_t      file_data_in_buffer = std::min(file_size, file_data.size());
    file.write(file_data.data(), file_data_in_buffer);
    file_size -= file_data_in_buffer;

    data = file_data.substr(file_data_in_buffer);
    while (file_size > 0) {
        std::vector<char> file_buffer(4096);
        std::size_t       bytes_to_read = std::min(static_cast<std::size_t>(4096), file_size);
        asio::error_code  ec;
        std::size_t       bytes_read =
            _socket->read_some(asio::buffer(file_buffer.data(), bytes_to_read), ec);

        if (ec) {
            std::cerr << "Error receiving file data: " << ec.message() << std::endl;
            return;
        }

        file.write(file_buffer.data(), bytes_read);
        file_size -= bytes_read;
    }

    file.close();
}

const std::vector<std::shared_ptr<asio::ip::tcp::socket>>& TCPPacketManager::get_client_sockets()
    const {
    return _client_sockets;
}

void TCPPacketManager::handle_directory_reception(const std::string& directory_name,
                                                  const std::string& parent_directory) {
    std::string full_path =
        parent_directory.empty() ? directory_name : parent_directory + "/" + directory_name;

    if (!std::filesystem::exists(full_path)) {
        try {
            std::filesystem::create_directories(full_path);
            std::cout << "Directory created: " << full_path << std::endl;
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Failed to create directory '" << full_path << "': " << e.what()
                      << std::endl;
            return;
        }
    }
}

void TCPPacketManager::listen_for_server_data() {
    std::vector<char> buffer(4096);
    asio::error_code  ec;

    std::string              current_directory;
    std::vector<std::string> directory_stack;

    while (true) {
        if (!_socket->is_open()) {
            std::cout << "Socket is closed. Exiting loop." << std::endl;
            break;
        }
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
            if (data.rfind("TARGET:", 0) == 0) {
                size_t end_pos = data.find('\n');
                if (end_pos == std::string::npos) {
                    std::cerr << "Malformed target header received." << std::endl;
                    break;
                }
                std::string target_directory = data.substr(7, end_pos - 7);
                current_directory            = target_directory;
                std::cout << "Setting client target directory to: " << current_directory
                          << std::endl;
                data = data.substr(end_pos + 1);

            } else if (data.rfind("MSG:", 0) == 0) {
                size_t end_pos = data.find('\n');
                if (end_pos == std::string::npos) {
                    std::cerr << "Malformed message received." << std::endl;
                    break;
                }
                std::string message = data.substr(4, end_pos - 4);
                std::cout << "Message received: " << message << std::endl;
                data = data.substr(end_pos + 1);
            } else if (data.rfind("FIN", 0) == 0) {
                std::cout << "Server finished sending data." << std::endl;
                _socket->close();
                _io_context.stop();
                return;  // Assurez-vous de sortir immédiatement de la fonction
            } else if (data.rfind("FILE:", 0) == 0) {
                handle_file_reception(data, current_directory);
            } else if (data.rfind("DIR:", 0) == 0) {
                size_t end_pos = data.find('\n');
                if (end_pos == std::string::npos) {
                    std::cerr << "Malformed directory header received." << std::endl;
                    break;
                }
                std::string dir_name = data.substr(4, end_pos - 4);
                std::string full_path =
                    current_directory.empty() ? dir_name : current_directory + "/" + dir_name;
                handle_directory_reception(dir_name, current_directory);
                directory_stack.push_back(current_directory);
                current_directory = full_path;
                data              = data.substr(end_pos + 1);
            } else if (data.rfind("ENDDIR", 0) == 0) {
                if (!directory_stack.empty()) {
                    current_directory = directory_stack.back();
                    directory_stack.pop_back();
                } else {
                    current_directory = "";
                }
                data = data.substr(7);
            } else {
                std::cerr << "Unknown data format received: " << data << std::endl;
                break;
            }
        }
    }
}

void TCPPacketManager::send_file_to_client(const std::string&             file_path,
                                           const asio::ip::tcp::endpoint& endpoint) {
    auto file = std::make_shared<std::ifstream>(file_path, std::ios::binary);
    if (!file->is_open()) {
        std::cerr << "Error opening file: " << file_path << std::endl;
        return;
    }
    file->seekg(0, std::ios::end);
    std::size_t file_size = file->tellg();
    file->seekg(0, std::ios::beg);
    std::string file_name     = file_path.substr(file_path.find_last_of("/\\") + 1);
    std::string header        = "FILE:" + file_name + ":" + std::to_string(file_size) + "\n";
    auto        header_buffer = std::make_shared<std::vector<char>>(header.begin(), header.end());
    std::cout << "Preparing to send file: " << file_name << " (" << file_size << " bytes)"
              << std::endl;
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
            }
            std::cout << "File sent successfully to " << endpoint << std::endl;
            break;
        }
    }
}

void TCPPacketManager::send_directory_to_client(const std::string&             directory_path,
                                                const asio::ip::tcp::endpoint& endpoint) {
    namespace fs = std::filesystem;

    if (!fs::exists(directory_path) || !fs::is_directory(directory_path)) {
        std::cerr << "Directory does not exist or is not valid: " << directory_path << std::endl;
        return;
    }

    std::string directory_name = fs::path(directory_path).filename().string();
    std::cout << "Directory name to send: [" << directory_name << "]" << std::endl;

    std::string header = "DIR:" + directory_name + "\n";
    for (auto& client_socket : _client_sockets) {
        if (client_socket->remote_endpoint() == endpoint) {
            asio::write(*client_socket, asio::buffer(header));
            break;
        }
    }
    for (const auto& entry : fs::directory_iterator(directory_path)) {
        if (entry.is_regular_file()) {
            send_file_to_client(entry.path().string(), endpoint);
        }
    }
    for (const auto& entry : fs::directory_iterator(directory_path)) {
        if (entry.is_directory()) {
            send_directory_to_client(entry.path().string(), endpoint);
        }
    }
    std::string end_dir_marker = "ENDDIR\n";
    for (auto& client_socket : _client_sockets) {
        if (client_socket->remote_endpoint() == endpoint) {
            asio::write(*client_socket, asio::buffer(end_dir_marker));
            break;
        }
    }

    std::cout << "Directory sent successfully: " << directory_name << std::endl;
}

void TCPPacketManager::send_directory_to_directory(const std::string&             directory_path,
                                                   const asio::ip::tcp::endpoint& endpoint,
                                                   const std::string& client_target_directory) {
    namespace fs = std::filesystem;

    if (!fs::exists(directory_path) || !fs::is_directory(directory_path)) {
        std::cerr << "Directory does not exist or is not valid: " << directory_path << std::endl;
        return;
    }

    std::string directory_name = fs::path(directory_path).filename().string();
    std::cout << "Sending directory: " << directory_name
              << " to client directory: " << client_target_directory << std::endl;
    std::string header = "TARGET:" + client_target_directory + "\n";
    for (auto& client_socket : _client_sockets) {
        if (client_socket->remote_endpoint() == endpoint) {
            asio::write(*client_socket, asio::buffer(header));
            break;
        }
    }
    send_directory_to_client(directory_path, endpoint);
}

void TCPPacketManager::close() {
    _io_context.stop();
    if (_io_thread.joinable()) {
        _io_thread.join();
    }
}
