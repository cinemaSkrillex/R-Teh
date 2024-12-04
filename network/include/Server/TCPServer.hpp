/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** TCPServer.hpp
*/

#ifndef TCPCLIENT_HPP
#define TCPCLIENT_HPP

#include "PacketUtils.hpp"
#include "TCPPacketManager.hpp"

class TCPServer {
   public:
    TCPServer(unsigned short port);
    ~TCPServer();

    void setNewClientCallback(
        const std::function<void(const asio::ip::tcp::endpoint& client_endpoint)>& callback);
    void setDisconnectClientCallback(
        const std::function<void(const asio::ip::tcp::endpoint& client_endpoint)>& callback);
    // void setReceiveMessageCallback(
    //     const std::function<void(const std::string&             message,
    //                              const asio::ip::tcp::endpoint& endpoint)>& callback);
    // void setReceiveFileCallback(
    //     const std::function<void(const std::string&             file_path,
    //                              const asio::ip::tcp::endpoint& endpoint)>& callback);

    void send_message(const std::string& message);
    void send_message(const std::string& message, const asio::ip::tcp::endpoint& endpoint);
    void send_file_to_client(const std::string& file_path, const asio::ip::tcp::endpoint& endpoint);

   private:
    std::shared_ptr<TCPPacketManager> _packet_manager;
};
#endif