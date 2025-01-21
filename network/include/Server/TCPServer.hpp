/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** TCPServer.hpp
*/

#ifndef TCPSERVER_HPP
#define TCPSERVER_HPP

#include <asio.hpp>
#include <functional>

#include "TCPPacketManager.hpp"

class TCPServer {
   public:
    TCPServer(unsigned short port);
    ~TCPServer();

    void setNewClientCallback(
        const std::function<void(const asio::ip::tcp::endpoint& client_endpoint)>& callback);
    void sendMessage(const std::string& message, const asio::ip::tcp::endpoint& endpoint);
    void send_file(const std::string& filePath, const asio::ip::tcp::endpoint& endpoint);
    void sendDirectory(const std::string& directoryPath, const asio::ip::tcp::endpoint& endpoint);
    void sendDirectoryToDirectory(const std::string&             directoryPath,
                                  const asio::ip::tcp::endpoint& endpoint,
                                  const std::string&             clientTargetDirectory);
    void sendFin(const asio::ip::tcp::endpoint& endpoint);

   private:
    std::shared_ptr<TCPPacketManager> _packetManager;
};

#endif
