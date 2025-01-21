/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** TCPServer.cpp
*/

#include "../../include/Server/TCPServer.hpp"

#include <iostream>

TCPServer::TCPServer(unsigned short port)
    : _packetManager(std::make_shared<TCPPacketManager>(Role::SERVER)) {
    _packetManager->startServer(port);
}

TCPServer::~TCPServer() { _packetManager->close(); }

void TCPServer::setNewClientCallback(
    const std::function<void(const asio::ip::tcp::endpoint& client_endpoint)>& callback) {
    _packetManager->_newClientCallback = callback;
}

void TCPServer::sendMessage(const std::string& message, const asio::ip::tcp::endpoint& endpoint) {
    _packetManager->sendMessageToClientEndpoint(message, endpoint);
}

void TCPServer::send_file(const std::string& filePath, const asio::ip::tcp::endpoint& endpoint) {
    _packetManager->sendFileToClient(filePath, endpoint);
}

void TCPServer::sendDirectory(const std::string&             directoryPath,
                              const asio::ip::tcp::endpoint& endpoint) {
    _packetManager->sendDirectoryToClient(directoryPath, endpoint);
}

void TCPServer::sendDirectoryToDirectory(const std::string&             directoryPath,
                                         const asio::ip::tcp::endpoint& endpoint,
                                         const std::string&             clientTargetDirectory) {
    _packetManager->sendDirectoryToDirectory(directoryPath, endpoint, clientTargetDirectory);
}

void TCPServer::sendFin(const asio::ip::tcp::endpoint& endpoint) {
    std::string fin_message = "FIN\n";
    for (auto& client_socket : _packetManager->getClientSockets()) {
        if (client_socket->remote_endpoint() == endpoint) {
            asio::write(*client_socket, asio::buffer(fin_message));
            client_socket->close();
            std::cout << "Sent FIN message to client and closed socket: " << endpoint << std::endl;
            break;
        }
    }
}
