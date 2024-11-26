/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** main
*/

#include <iostream>
#include <asio.hpp>
#include "include/DynamicLibrary/DynamicLibrary.hpp"
#include "include/Server/UDPServer.hpp"

#if defined(_WIN32) || defined(_WIN64)
#define _WIN32_WINNT 0x0A00
#endif

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    unsigned short port = static_cast<unsigned short>(std::stoi(argv[1]));

    try {
        asio::io_context io_context;
        // auto             test_server = std::make_shared<TestServer>(io_context, port);

        // test_server->start();
        // io_context.run();
        auto server = std::make_shared<UDPServer>(io_context, port);

        // Run the io_context to handle asynchronous operations
        io_context.run();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}