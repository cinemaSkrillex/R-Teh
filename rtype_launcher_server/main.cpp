/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** main.cpp for File Sharing Server
*/

#include <asio.hpp>
#include <filesystem>
#include <iostream>
#include <thread>

#include "../include/Server/TCPServer.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    unsigned short port = static_cast<unsigned short>(std::stoi(argv[1]));

    try {
        asio::io_context io_context;
        auto             tcp_server = std::make_shared<TCPServer>(port);
        tcp_server->setNewClientCallback(
            [tcp_server](const asio::ip::tcp::endpoint& client_endpoint) {
                std::cout << "New client connected: " << client_endpoint << std::endl;
                tcp_server->send_message("UWU1", client_endpoint);
                tcp_server->send_directory("../../../test", client_endpoint);
                tcp_server->send_message("T'as tout les fichiers", client_endpoint);
            });
        io_context.run();
        std::this_thread::sleep_for(std::chrono::hours(1));
        std::cout << "Server stopped" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
