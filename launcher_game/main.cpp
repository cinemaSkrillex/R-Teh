/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** TestingClientMain.cpp
*/

#include <asio.hpp>
#include <iostream>
#include <thread>

#include "TCPClient.hpp"
#include "DynamicLibrary/DynamicLibrary.hpp"

#if defined(_WIN32) || defined(_WIN64)
#define _WIN32_WINNT 0x0A00
#endif

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <dlfcn.h>
#endif

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <server_ip> <server_port>"
                  << std::endl;
        return 1;
    }

    std::string    server_ip   = argv[1];
    unsigned short server_port = static_cast<unsigned short>(std::stoi(argv[2]));

    try {
        std::cout << "Starting client" << std::endl;
        asio::io_context io_context;

        auto tcpclient = std::make_shared<TCPClient>(server_ip, server_port);
        std::this_thread::sleep_for(std::chrono::seconds(100000000));
        std::cout << "Client stopped" << std::endl;
        exit(0);
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}