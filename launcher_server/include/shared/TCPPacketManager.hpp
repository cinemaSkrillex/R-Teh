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

    void start_server(unsigned short port);
    void start_client(const std::string& host, unsigned short port);
    void listen_for_server_data();
    void send_message_to_server(const std::string& message);
    void send_message_to_client_endpoint(const std::string&             message,
                                         const asio::ip::tcp::endpoint& endpoint);
    void send_file_to_client(const std::string& file_path, const asio::ip::tcp::endpoint& endpoint);
    void send_directory_to_client(const std::string&             directory_path,
                                  const asio::ip::tcp::endpoint& endpoint);
    void handle_file_reception(std::string& data, const std::string& current_directory);
    void handle_directory_reception(const std::string& directory_name,
                                    const std::string& parent_directory);
    void close();

    std::function<void(const asio::ip::tcp::endpoint& client_endpoint)> _new_client_callback;

   private:
    void accept_clients(std::shared_ptr<asio::ip::tcp::acceptor> acceptor);

    Role                                                       _role;
    asio::io_context                                           _io_context;
    std::shared_ptr<asio::ip::tcp::socket>                     _socket;
    std::vector<std::shared_ptr<asio::ip::tcp::socket>>        _client_sockets;
    asio::executor_work_guard<asio::io_context::executor_type> _work_guard;
    std::thread                                                _io_thread;
};

#endif
