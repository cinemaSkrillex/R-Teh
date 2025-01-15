/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** TCPServer.cpp
*/

#include "../../include/Server/TCPServer.hpp"

#include <iostream>

TCPServer::TCPServer(unsigned short port)
    : _packet_manager(std::make_shared<TCPPacketManager>(Role::SERVER)) {
    _packet_manager->start_server(port);
}

TCPServer::~TCPServer() { _packet_manager->close(); }

void TCPServer::setNewClientCallback(
    const std::function<void(const asio::ip::tcp::endpoint& client_endpoint)>& callback) {
    _packet_manager->_new_client_callback = callback;
}

void TCPServer::send_message(const std::string& message, const asio::ip::tcp::endpoint& endpoint) {
    _packet_manager->send_message_to_client_endpoint(message, endpoint);
}

void TCPServer::send_file(const std::string& file_path, const asio::ip::tcp::endpoint& endpoint) {
    _packet_manager->send_file_to_client(file_path, endpoint);
}

void TCPServer::send_directory(const std::string&             directory_path,
                               const asio::ip::tcp::endpoint& endpoint) {
    _packet_manager->send_directory_to_client(directory_path, endpoint);
}

void TCPServer::send_directory_to_directory(const std::string&             directory_path,
                                            const asio::ip::tcp::endpoint& endpoint,
                                            const std::string& client_target_directory) {
    _packet_manager->send_directory_to_directory(directory_path, endpoint, client_target_directory);
}

void TCPServer::send_fin(const asio::ip::tcp::endpoint& endpoint) {
    std::string fin_message = "FIN\n";
    for (auto& client_socket : _packet_manager->get_client_sockets()) {
        if (client_socket->remote_endpoint() == endpoint) {
            asio::write(*client_socket, asio::buffer(fin_message));
            client_socket->close();  // Fermez le socket après l'envoi de "FIN"
            std::cout << "Sent FIN message to client and closed socket: " << endpoint << std::endl;
            break;
        }
    }
}
