/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** TCPPacketManager.hpp
*/

#ifndef TCPPACKETMANAGER_HPP
#define TCPPACKETMANAGER_HPP

#include <iostream>
#include <memory>
#include <string>

#include "TCPPacketUtils.hpp"

class TCPPacketManager {
   public:
    TCPPacketManager(Role role);

    // todo callback
    // packets binary protocol
    // get last message
    //  get all message
    // disconnect player
    void start_server(unsigned short port);
    void start_client(const std::string& host, short port);

    void listen_for_client_data(std::shared_ptr<asio::ip::tcp::socket> client_socket);
    void listen_for_server_data();

    void accept_clients(std::shared_ptr<asio::ip::tcp::acceptor> acceptor);

    void send_message(const std::string& message);

    void close();

   private:
    asio::io_context      _io_context;
    asio::ip::tcp::socket _socket;
    packet                _pkt;

    enum { max_length = 1024 };
    char           data_[max_length];
    Role           _role;
    std::string    _server_ip;
    unsigned short _server_port;
};

#endif  // TCPPACKETMANAGER_HPP