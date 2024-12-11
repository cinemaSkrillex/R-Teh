/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** TCPServer.hpp
*/

#ifndef TCPSERVER_HPP
#define TCPSERVER_HPP

#include "TCPPacketManager.hpp"
#include <functional>
#include <asio.hpp>

class TCPServer {
   public:
    TCPServer(unsigned short port);
    ~TCPServer();

    void setNewClientCallback(
        const std::function<void(const asio::ip::tcp::endpoint& client_endpoint)>& callback);
    void send_message(const std::string& message, const asio::ip::tcp::endpoint& endpoint);
    void send_file(const std::string& file_path, const asio::ip::tcp::endpoint& endpoint);

   private:
    std::shared_ptr<TCPPacketManager> _packet_manager;
};

#endif
