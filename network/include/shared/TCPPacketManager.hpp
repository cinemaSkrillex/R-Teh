/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** TCPPacketManager.hpp
*/

#ifndef TCPPACKETMANAGER_HPP
#define TCPPACKETMANAGER_HPP

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#include "TCPPacketUtils.hpp"

class TCPPacketManager : public std::enable_shared_from_this<TCPPacketManager> {
   public:
    TCPPacketManager(Role role);
    ~TCPPacketManager() { close(); }

    // todo callback
    // packets binary protocol
    // send message avec un endpoint
    // check si besoin de 1 thread par nouveaux clients
    // get last message
    //  get all message
    // disconnect player
    void start_server(unsigned short port);
    void start_client(const std::string& host, short port);

    void listen_for_client_data(std::shared_ptr<asio::ip::tcp::socket> client_socket);
    void listen_for_server_data();

    void accept_clients(std::shared_ptr<asio::ip::tcp::acceptor> acceptor);

    void send_message(const std::string& message);
    void send_message(const std::string& message, const asio::ip::tcp::endpoint& endpoint);
    void client_send_message(const std::string& message);

    // new functions to send larger files
    void send_packet(std::shared_ptr<asio::ip::tcp::socket> socket, const TCPPacket& packet);
    void send_file(const std::string& file_path, std::shared_ptr<asio::ip::tcp::socket> socket);
    void receive_file(std::shared_ptr<asio::ip::tcp::socket> socket, const std::string& save_dir);
    void close();

    // callbacks
    std::function<void(const asio::ip::tcp::endpoint& client_endpoint)> _new_client_callback;
    std::function<void(const asio::ip::tcp::endpoint& client_endpoint)> _disconnect_client_callback;
    std::function<void(const std::string& message, const asio::ip::tcp::endpoint& endpoint)>
        _receive_message_callback;
    std::function<void(const std::string& file_path, const asio::ip::tcp::endpoint& endpoint)>
        _receive_file_callback;

   private:
    asio::io_context                                           _io_context;
    std::shared_ptr<asio::ip::tcp::socket>                     _socket;
    asio::executor_work_guard<asio::io_context::executor_type> _work_guard;
    packet                                                     _pkt;
    std::vector<std::shared_ptr<asio::ip::tcp::socket>>        _client_sockets;
    std::thread                                                _io_thread;

    enum { max_length = 1024 };
    char           data_[max_length];
    Role           _role;
    std::string    _server_ip;
    unsigned short _server_port;
};

#endif  // TCPPACKETMANAGER_HPP