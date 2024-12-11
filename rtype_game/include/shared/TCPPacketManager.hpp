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
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "PacketUtils.hpp"

enum class TCPFlags { MESSAGE = 0, IMAGE = 1 };

class TCPPacketManager : public std::enable_shared_from_this<TCPPacketManager> {
   public:
    TCPPacketManager(Role role);
    ~TCPPacketManager() { close(); }

    // todo callback (receive message)
    // packets binary protocol
    // send message avec un endpoint
    // check si besoin de 1 thread par nouveaux clients
    // get last message
    //  get all message
    void start_server(unsigned short port);
    void start_client(const std::string& host, short port);

    void listen_for_client_data(std::shared_ptr<asio::ip::tcp::socket> client_socket);
    void listen_for_server_data();

    void accept_clients(std::shared_ptr<asio::ip::tcp::acceptor> acceptor);

    void send_message_to_client(const std::string& message);
    void send_message_to_client_endpoint(const std::string&             message,
                                         const asio::ip::tcp::endpoint& endpoint);
    void send_message_to_server(const std::string& message);

    // new functions to send larger files
    void close();

    void send_file_to_client(const std::string& file_path, const asio::ip::tcp::endpoint& endpoint);

    std::shared_ptr<std::vector<char>> serialize_string(const std::string& message, TCPFlags flag, const std::string& file_name = "");

    // build packet
    // TODO when PACKET WORKS

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
    std::vector<std::shared_ptr<asio::ip::tcp::socket>>        _client_sockets;
    std::thread                                                _io_thread;

    std::array<char, 1024> recv_buffer_;

    Role           _role;
    std::string    _server_ip;
    unsigned short _server_port;

    // handle receive
    void handle_receive(std::size_t bytes_recvd);
    void receive_file_data(std::shared_ptr<std::vector<char>> file_buffer, std::size_t total_size,
                           std::size_t bytes_received, const std::string& file_name);
    void send_message(std::shared_ptr<std::vector<char>>     serialized_message,
                      std::shared_ptr<asio::ip::tcp::socket> receiver_socket);
};

#endif  // TCPPACKETMANAGER_HPP