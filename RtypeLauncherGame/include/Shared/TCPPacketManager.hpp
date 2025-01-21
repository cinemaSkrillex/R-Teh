/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** TCPPacketManager.hpp
*/

#ifndef TCPPACKETMANAGER_HPP
#define TCPPACKETMANAGER_HPP

#include <asio.hpp>
#include <filesystem>
#include <fstream>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "PacketUtils.hpp"

// enum class Role { SERVER, CLIENT };

class TCPPacketManager {
   public:
    TCPPacketManager(Role role);

    void startServer(unsigned short port);
    void startClient(const std::string& host, unsigned short port);
    void listenForServerData();
    void sendMessageToServer(const std::string& message);
    void sendMessageToClientEndpoint(const std::string&             message,
                                     const asio::ip::tcp::endpoint& endpoint);
    void sendFileToClient(const std::string& filePath, const asio::ip::tcp::endpoint& endpoint);
    void sendDirectoryToClient(const std::string&             directoryPath,
                               const asio::ip::tcp::endpoint& endpoint);
    void handleFileReception(std::string& data, const std::string& currentDirectory);
    void handleDirectoryReception(const std::string& directoryName,
                                  const std::string& parent_directory);
    void sendDirectoryToDirectory(const std::string&             directoryPath,
                                  const asio::ip::tcp::endpoint& endpoint,
                                  const std::string&             targetDirectory);
    void close();

    std::function<void(const asio::ip::tcp::endpoint& client_endpoint)> _newClientCallback;
    const std::vector<std::shared_ptr<asio::ip::tcp::socket>>&          getClientSockets() const;

   private:
    void accept_clients(std::shared_ptr<asio::ip::tcp::acceptor> acceptor);

    Role                                                       _role;
    asio::io_context                                           _ioContext;
    std::shared_ptr<asio::ip::tcp::socket>                     _socket;
    std::vector<std::shared_ptr<asio::ip::tcp::socket>>        _clientSockets;
    asio::executor_work_guard<asio::io_context::executor_type> _workGuard;
    std::thread                                                _ioThread;
};

#endif
