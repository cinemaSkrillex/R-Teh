/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** TCPPacketManager.hpp
*/

#ifndef TCPPACKETMANAGER_HPP
#define TCPPACKETMANAGER_HPP

#include "PacketUtils.hpp"
#include <asio.hpp>
#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <filesystem>
#include <fstream>

// enum class Role { SERVER, CLIENT };

class TCPPacketManager {
   public:
    TCPPacketManager(Role role);

    void start_server(unsigned short port);
    void start_client(const std::string& host, unsigned short port);
    void listen_for_server_data();
    void send_message_to_server(const std::string& message);
    void send_message_to_client_endpoint(const std::string& message,
                                         const asio::ip::tcp::endpoint& endpoint);
    void send_file_to_client(const std::string& file_path, const asio::ip::tcp::endpoint& endpoint);
    void receive_file_data(std::shared_ptr<std::ofstream> file, std::size_t expected_size);
    void close();

    std::function<void(const asio::ip::tcp::endpoint& client_endpoint)> _new_client_callback;

   private:
    void accept_clients(std::shared_ptr<asio::ip::tcp::acceptor> acceptor);

    Role _role;
    asio::io_context _io_context;
    std::shared_ptr<asio::ip::tcp::socket> _socket;
    std::vector<std::shared_ptr<asio::ip::tcp::socket>> _client_sockets;
    asio::executor_work_guard<asio::io_context::executor_type> _work_guard;
    std::thread _io_thread;
};

#endif
