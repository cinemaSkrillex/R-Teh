/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** main.cpp
*/

#include <asio.hpp>
#include <iostream>

#include "include/DynamicLibrary/DynamicLibrary.hpp"
#include "include/Server/TCPServer.hpp"
#include "include/Server/UDPServer.hpp"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <dlfcn.h>
#endif

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    unsigned short port = static_cast<unsigned short>(std::stoi(argv[1]));

    try {
        // asio::io_context io_context;
        // TCP
        auto server = std::make_shared<TCPServer>(port);
        std::cout << "Server " << port << std::endl;
        server->setNewClientCallback([server](const asio::ip::tcp::endpoint& client_endpoint) {
            std::cout << "New client connected: " << client_endpoint << std::endl;
            server->send_message("Hello from server! after clientFSkozeoigz");

            server->send_message("I sent ur endoint", client_endpoint);
        });
        server->setDisconnectClientCallback(
            [server](const asio::ip::tcp::endpoint& client_endpoint) {
                std::cout << "Client disconnected from server I detected it: " << client_endpoint
                          << std::endl;
            });
        std::this_thread::sleep_for(std::chrono::hours(1));  // For example, wait for 1 hour

        std::cout << "end of server " << port << std::endl;
        // UDP
        //  auto             server = std::make_shared<UDPServer>(io_context, port);

        // io_context.run();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
