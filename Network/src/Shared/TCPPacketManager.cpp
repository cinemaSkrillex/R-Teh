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
      _socket(std::make_shared<asio::ip::tcp::socket>(_ioContext)),
      _workGuard(asio::make_work_guard(_ioContext)) {}

void TCPPacketManager::startServer(unsigned short port) {
    auto acceptor = std::make_shared<asio::ip::tcp::acceptor>(
        _ioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port));
    acceptClients(acceptor);
    _ioThread = std::thread([this]() { _ioContext.run(); });
}

void TCPPacketManager::startClient(const std::string& host, unsigned short port) {
    asio::ip::tcp::resolver resolver(_ioContext);
    auto                    endpoints = resolver.resolve(host, std::to_string(port));
    asio::connect(*_socket, endpoints);
    std::cout << "Connected to server at " << host << ":" << port << std::endl;
    listenForServerData();

    _ioThread = std::thread([this]() { _ioContext.run(); });
}

void TCPPacketManager::acceptClients(std::shared_ptr<asio::ip::tcp::acceptor> acceptor) {
    auto new_socket = std::make_shared<asio::ip::tcp::socket>(_ioContext);
    acceptor->async_accept(*new_socket, [this, acceptor, new_socket](asio::error_code ec) {
        if (!ec) {
            _clientSockets.push_back(new_socket);
            if (_newClientCallback) {
                _newClientCallback(new_socket->remote_endpoint());
            }
        } else {
            std::cerr << "acceptClients: Error accepting client: " << ec.message() << std::endl;
        }
        acceptClients(acceptor);
    });
}

void TCPPacketManager::sendMessageToServer(const std::string& message) {
    asio::async_write(*_socket, asio::buffer(message), [](std::error_code ec, std::size_t) {
        if (ec) {
            std::cerr << "sendMessageToServer: Error sending message: " << ec.message()
                      << std::endl;
        }
    });
}

void TCPPacketManager::sendMessageToClientEndpoint(const std::string&             message,
                                                   const asio::ip::tcp::endpoint& endpoint) {
    for (auto& client_socket : _clientSockets) {
        if (!client_socket->is_open()) {
            std::cerr << "sendMessageToClientEndpoint: Socket is not open. Cannot send message to "
                      << endpoint << std::endl;
            continue;
        }
        if (client_socket->remote_endpoint() == endpoint) {
            std::string formatted_message = "MSG:" + message + "\n";
            std::cout << "Sending message to client: " << message << std::endl;
            try {
                asio::write(*client_socket, asio::buffer(formatted_message));
                std::cout << "Message sent successfully." << std::endl;
            } catch (const std::system_error& e) {
                std::cerr << "sendMessageToClientEndpoint: Error sending message: " << e.what()
                          << std::endl;
            }
            break;
        }
    }
}

void TCPPacketManager::handleFileReception(std::string& data, const std::string& currentDirectory) {
    namespace fs = std::filesystem;

    size_t name_end = data.find(':', 5);
    size_t size_end = data.find('\n', name_end + 1);

    if (name_end == std::string::npos || size_end == std::string::npos) {
        std::cerr << "handleFileReception: Malformed file header received." << std::endl;
        return;
    }

    std::string file_name = data.substr(5, name_end - 5);
    std::size_t file_size = std::stoul(data.substr(name_end + 1, size_end - name_end - 1));
    std::cout << "Receiving file: " << file_name << " (" << file_size << " bytes)" << std::endl;
    std::string directory = currentDirectory.empty() ? "FILE_SHARING" : currentDirectory;

    if (!fs::exists(directory)) {
        try {
            fs::create_directory(directory);
            std::cout << "Directory created: " << directory << std::endl;
        } catch (const fs::filesystem_error& e) {
            std::cerr << "handleFileReception: Failed to create directory '" << directory
                      << "': " << e.what() << std::endl;
            return;
        }
    }

    std::string filePath = directory + "/" + file_name;

    std::ofstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "handleFileReception: Failed to open file for writing: " << filePath
                  << std::endl;
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
            std::cerr << "handleFileReception: Error receiving file data: " << ec.message()
                      << std::endl;
            return;
        }

        file.write(file_buffer.data(), bytes_read);
        file_size -= bytes_read;
    }

    file.close();
}

const std::vector<std::shared_ptr<asio::ip::tcp::socket>>& TCPPacketManager::getClientSockets()
    const {
    return _clientSockets;
}

void TCPPacketManager::handleDirectoryReception(const std::string& directoryName,
                                                const std::string& parent_directory) {
    std::string full_path =
        parent_directory.empty() ? directoryName : parent_directory + "/" + directoryName;

    if (!std::filesystem::exists(full_path)) {
        try {
            std::filesystem::create_directories(full_path);
            std::cout << "Directory created: " << full_path << std::endl;
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "handleDirectoryReception: Failed to create directory '" << full_path
                      << "': " << e.what() << std::endl;
            return;
        }
    }
}

void TCPPacketManager::listenForServerData() {
    std::vector<char> buffer(4096);
    asio::error_code  ec;

    std::string              currentDirectory;
    std::vector<std::string> directory_stack;

    while (true) {
        if (!_socket->is_open()) {
            std::cout << "listenForServerData: Socket is closed. Exiting loop." << std::endl;
            break;
        }
        std::size_t length = _socket->read_some(asio::buffer(buffer), ec);
        if (ec) {
            if (ec == asio::error::eof) {
                std::cout << "listenForServerData: Connection closed by server." << std::endl;
                break;
            } else {
                std::cerr << "listenForServerData: Error reading from server: " << ec.message()
                          << std::endl;
                break;
            }
        }

        std::string data(buffer.data(), length);
        while (!data.empty()) {
            if (data.rfind("TARGET:", 0) == 0) {
                size_t end_pos = data.find('\n');
                if (end_pos == std::string::npos) {
                    std::cerr << "listenForServerData: Malformed target header received."
                              << std::endl;
                    break;
                }
                std::string targetDirectory = data.substr(7, end_pos - 7);
                currentDirectory            = targetDirectory;
                std::cout << "Setting client target directory to: " << currentDirectory
                          << std::endl;
                data = data.substr(end_pos + 1);

            } else if (data.rfind("MSG:", 0) == 0) {
                size_t end_pos = data.find('\n');
                if (end_pos == std::string::npos) {
                    std::cerr << "listenForServerData: Malformed message received." << std::endl;
                    break;
                }
                std::string message = data.substr(4, end_pos - 4);
                std::cout << "Message received: " << message << std::endl;
                data = data.substr(end_pos + 1);
            } else if (data.rfind("FIN", 0) == 0) {
                std::cout << "Server finished sending data." << std::endl;
                _socket->close();
                _ioContext.stop();
                return;  // Assurez-vous de sortir immédiatement de la fonction
            } else if (data.rfind("FILE:", 0) == 0) {
                handleFileReception(data, currentDirectory);
            } else if (data.rfind("DIR:", 0) == 0) {
                size_t end_pos = data.find('\n');
                if (end_pos == std::string::npos) {
                    std::cerr << "listenForServerData: Malformed directory header received."
                              << std::endl;
                    break;
                }
                std::string dir_name = data.substr(4, end_pos - 4);
                std::string full_path =
                    currentDirectory.empty() ? dir_name : currentDirectory + "/" + dir_name;
                handleDirectoryReception(dir_name, currentDirectory);
                directory_stack.push_back(currentDirectory);
                currentDirectory = full_path;
                data             = data.substr(end_pos + 1);
            } else if (data.rfind("ENDDIR", 0) == 0) {
                if (!directory_stack.empty()) {
                    currentDirectory = directory_stack.back();
                    directory_stack.pop_back();
                } else {
                    currentDirectory = "";
                }
                data = data.substr(7);
            } else {
                std::cerr << "listenForServerData: Unknown data format received: " << data
                          << std::endl;
                break;
            }
        }
    }
}

void TCPPacketManager::sendFileToClient(const std::string&             filePath,
                                        const asio::ip::tcp::endpoint& endpoint) {
    auto file = std::make_shared<std::ifstream>(filePath, std::ios::binary);
    if (!file->is_open()) {
        std::cerr << "sendFileToClient: Error opening file: " << filePath << std::endl;
        return;
    }
    file->seekg(0, std::ios::end);
    std::size_t file_size = file->tellg();
    file->seekg(0, std::ios::beg);
    std::string file_name     = filePath.substr(filePath.find_last_of("/\\") + 1);
    std::string header        = "FILE:" + file_name + ":" + std::to_string(file_size) + "\n";
    auto        header_buffer = std::make_shared<std::vector<char>>(header.begin(), header.end());
    std::cout << "Preparing to send file: " << file_name << " (" << file_size << " bytes)"
              << std::endl;
    for (auto& client_socket : _clientSockets) {
        if (client_socket->remote_endpoint() == endpoint) {
            if (!client_socket->is_open()) {
                std::cerr << "sendFileToClient: Socket is not open for endpoint: " << endpoint
                          << std::endl;
                return;
            }
            asio::write(*client_socket, asio::buffer(*header_buffer));
            auto chunk_buffer = std::make_shared<std::vector<char>>(4096);
            while (!file->eof()) {
                file->read(chunk_buffer->data(), chunk_buffer->size());
                std::streamsize bytes_read = file->gcount();
                if (!client_socket->is_open()) {
                    std::cerr
                        << "sendFileToClient: Socket closed during file transmission for endpoint: "
                        << endpoint << std::endl;
                    return;
                }
                asio::write(*client_socket, asio::buffer(chunk_buffer->data(), bytes_read));
            }
            std::cout << "File sent successfully to " << endpoint << std::endl;
            break;
        }
    }
}

void TCPPacketManager::sendDirectoryToClient(const std::string&             directoryPath,
                                             const asio::ip::tcp::endpoint& endpoint) {
    namespace fs = std::filesystem;

    if (!fs::exists(directoryPath) || !fs::is_directory(directoryPath)) {
        std::cerr << "sendDirectoryToClient: Directory does not exist or is not valid: "
                  << directoryPath << std::endl;
        return;
    }

    std::string directoryName = fs::path(directoryPath).filename().string();
    std::cout << "Directory name to send: [" << directoryName << "]" << std::endl;

    std::string header = "DIR:" + directoryName + "\n";
    for (auto& client_socket : _clientSockets) {
        if (client_socket->remote_endpoint() == endpoint) {
            asio::write(*client_socket, asio::buffer(header));
            break;
        }
    }
    for (const auto& entry : fs::directory_iterator(directoryPath)) {
        if (entry.is_regular_file()) {
            sendFileToClient(entry.path().string(), endpoint);
        }
    }
    for (const auto& entry : fs::directory_iterator(directoryPath)) {
        if (entry.is_directory()) {
            sendDirectoryToClient(entry.path().string(), endpoint);
        }
    }
    std::string end_dir_marker = "ENDDIR\n";
    for (auto& client_socket : _clientSockets) {
        if (client_socket->remote_endpoint() == endpoint) {
            asio::write(*client_socket, asio::buffer(end_dir_marker));
            break;
        }
    }

    std::cout << "Directory sent successfully: " << directoryName << std::endl;
}

void TCPPacketManager::sendDirectoryToDirectory(const std::string&             directoryPath,
                                                const asio::ip::tcp::endpoint& endpoint,
                                                const std::string& clientTargetDirectory) {
    namespace fs = std::filesystem;

    if (!fs::exists(directoryPath) || !fs::is_directory(directoryPath)) {
        std::cerr << "sendDirectoryToDirectory: Directory does not exist or is not valid: "
                  << directoryPath << std::endl;
        return;
    }

    std::string directoryName = fs::path(directoryPath).filename().string();
    std::cout << "Sending directory: " << directoryName
              << " to client directory: " << clientTargetDirectory << std::endl;
    std::string header = "TARGET:" + clientTargetDirectory + "\n";
    for (auto& client_socket : _clientSockets) {
        if (client_socket->remote_endpoint() == endpoint) {
            asio::write(*client_socket, asio::buffer(header));
            break;
        }
    }
    sendDirectoryToClient(directoryPath, endpoint);
}

void TCPPacketManager::close() {
    _ioContext.stop();
    if (_ioThread.joinable()) {
        _ioThread.join();
    }
}
