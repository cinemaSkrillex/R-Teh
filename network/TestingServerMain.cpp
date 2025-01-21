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
        asio::io_context ioContext;
        auto             tcpserver = std::make_shared<TCPServer>(port);
        tcpserver->setNewClientCallback([tcpserver](
                                            const asio::ip::tcp::endpoint& client_endpoint) {
            std::cout << "New client connected: " << client_endpoint << std::endl;
            // tcpserver->sendDirectory("../../../assets", client_endpoint);
            tcpserver->sendDirectory("../rtype_game", client_endpoint);
            tcpserver->sendDirectoryToDirectory("../../../assets", client_endpoint, "rtype_game");
            std::this_thread::sleep_for(std::chrono::seconds(5));
            tcpserver->sendFin(client_endpoint);
        });
        // auto             server = std::make_shared<UDPServer>(ioContext, port);

        // for how long the server will run:
        std::this_thread::sleep_for(std::chrono::seconds(10000000));
        std::cout << "Server stopped" << std::endl;
        exit(0);
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}