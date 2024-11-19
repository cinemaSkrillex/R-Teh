/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** main.cpp
*/

#include <iostream>
#include <asio.hpp>
#include "UDPServer.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    unsigned short port = static_cast<unsigned short>(std::stoi(argv[1]));

    try {
        asio::io_context io_context;
        UDPServer        server(io_context, port);
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}