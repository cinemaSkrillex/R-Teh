/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** TCPServer.cpp
*/

#include "../../include/Server/TCPServer.hpp"

// TCPServer::TCPServer(unsigned short port) : _packet_manager(Role::SERVER) {
//     _packet_manager.start_server(port);
// }

TCPServer::TCPServer(unsigned short port)
    : _packet_manager(std::make_shared<TCPPacketManager>(Role::SERVER)) {
    _packet_manager->start_server(port);
}

TCPServer::~TCPServer() { _packet_manager->close(); }

void TCPServer::send_message(const std::string& message) {
    _packet_manager->send_message_to_client(message);
}

void TCPServer::send_message(const std::string& message, const asio::ip::tcp::endpoint& endpoint) {
    _packet_manager->send_message_to_client_endpoint(message, endpoint);
}

void TCPServer::send_file_to_client(const std::string&             file_path,
                                    const asio::ip::tcp::endpoint& endpoint) {
    _packet_manager->send_file_to_client(file_path, endpoint);
}

void TCPServer::setNewClientCallback(
    const std::function<void(const asio::ip::tcp::endpoint& client_endpoint)>& callback) {
    _packet_manager->_new_client_callback = callback;
}

void TCPServer::setDisconnectClientCallback(
    const std::function<void(const asio::ip::tcp::endpoint& client_endpoint)>& callback) {
    _packet_manager->_disconnect_client_callback = callback;
}
