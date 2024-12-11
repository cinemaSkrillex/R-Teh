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

    // Démarrez l'écoute pour recevoir des données du serveur
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
        if (!client_socket->is_open()) {  // Vérifiez si le socket est ouvert
            std::cerr << "Socket is not open. Cannot send message to " << endpoint << std::endl;
            continue;
        }

        if (client_socket->remote_endpoint() == endpoint) {
            std::cout << "Sending message to client: " << message << std::endl;
            asio::async_write(*client_socket, asio::buffer(message),
                              [](std::error_code ec, std::size_t) {
                                  if (ec) {
                                      std::cerr << "Error sending message: " << ec.message()
                                                << std::endl;
                                  } else {
                                      std::cout << "Message sent successfully." << std::endl;
                                  }
                              });
            break;
        }
    }
}


void TCPPacketManager::receive_file_data(std::shared_ptr<std::ofstream> file,
                                         std::size_t expected_size,
                                         std::size_t received_size) {
    auto buffer = std::make_shared<std::vector<char>>(4096); // Taille du chunk
    _socket->async_read_some(
        asio::buffer(*buffer),
        [this, file, buffer, expected_size, received_size](std::error_code ec, std::size_t length) {
            if (!ec) {
                file->write(buffer->data(), length); // Écrivez les données dans le fichier
                std::size_t total_received = received_size + length;

                std::cout << "Received chunk: " << length << " bytes (Total: " << total_received
                          << "/" << expected_size << ")" << std::endl;

                if (total_received < expected_size) {
                    // Continuez à lire les données manquantes
                    receive_file_data(file, expected_size, total_received);
                } else {
                    // Fichier reçu complètement
                    file->close();
                    std::cout << "File received and saved successfully." << std::endl;

                    // Recommencez à écouter pour d'autres données
                    listen_for_server_data();
                }
            } else {
                std::cerr << "Error receiving file data: " << ec.message() << std::endl;
            }
        });
}


void TCPPacketManager::listen_for_server_data() {
    auto buffer = std::make_shared<std::vector<char>>(4096); // Buffer temporaire
    _socket->async_read_some(
        asio::buffer(*buffer),
        [this, buffer](std::error_code ec, std::size_t length) {
            if (!ec) {
                std::string data(buffer->begin(), buffer->begin() + length);

                // Vérifiez si c'est un fichier ou un message
                if (data.rfind("MSG:", 0) == 0) {
                    std::string message = data.substr(4); // Retirez le préfixe
                    std::cout << "Message received: " << message << std::endl;

                } else if (data.rfind("FILE:", 0) == 0) {
                    // Détectez l'en-tête fichier
                    size_t name_end = data.find(':', 5);
                    size_t size_end = data.find('\n', name_end + 1);

                    std::string file_name = data.substr(5, name_end - 5);
                    std::size_t file_size = std::stoul(data.substr(name_end + 1, size_end - name_end - 1));

                    std::cout << "Receiving file: " << file_name << " (" << file_size << " bytes)" << std::endl;

                    // Créez le fichier pour écrire
                    auto file = std::make_shared<std::ofstream>("test/" + file_name, std::ios::binary);
                    if (!file->is_open()) {
                        std::cerr << "Failed to open file for writing: " << file_name << std::endl;
                        return;
                    }

                    // Passez à la lecture des données du fichier
                    receive_file_data(file, file_size, 0);
                    return;
                }

                // Continuez à écouter pour d'autres données
                listen_for_server_data();
            } else {
                std::cerr << "Error reading from server: " << ec.message() << std::endl;
            }
        });
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

    // Envoyez l'en-tête
    for (auto& client_socket : _client_sockets) {
        if (client_socket->remote_endpoint() == endpoint) {
            if (!client_socket->is_open()) {  // Vérifiez si le socket est valide
                std::cerr << "Socket is not open for endpoint: " << endpoint << std::endl;
                return;
            }

            // Envoyer l'en-tête
            asio::write(*client_socket, asio::buffer(*header_buffer));

            // Envoi des données par morceaux
            auto chunk_buffer = std::make_shared<std::vector<char>>(4096);  // Taille d'un chunk
            while (!file->eof()) {
                file->read(chunk_buffer->data(), chunk_buffer->size());
                std::streamsize bytes_read = file->gcount();

                if (!client_socket->is_open()) {  // Vérifiez le socket avant chaque envoi
                    std::cerr << "Socket closed during file transmission for endpoint: " << endpoint
                              << std::endl;
                    return;
                }

                asio::write(*client_socket, asio::buffer(chunk_buffer->data(), bytes_read));
                std::cout << "Sent chunk: " << bytes_read << " bytes" << std::endl;
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
