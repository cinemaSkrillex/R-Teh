/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** TestingClientMain.cpp
*/

#include <asio.hpp>
#include <iostream>

#include "include/Client/TCPClient.hpp"
#include "include/Client/UDPClient.hpp"
#include "include/DynamicLibrary/DynamicLibrary.hpp"

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
    // unsigned short client_port = static_cast<unsigned short>(std::stoi(argv[3]));

    try {
        std::cout << "Starting client" << std::endl;
        asio::io_context io_context;

        auto tcpclient = std::make_shared<TCPClient>(server_ip, server_port);

        // auto client = std::make_shared<UDPClient>(io_context, server_ip, server_port);
        // client->send_new_client();
        // client->send_test();
        // client->send_unreliable_packet("Hello");
        // client->send_unreliable_packet("World");
        // client->send_reliable_packet("Hello1");
        // client->send_unreliable_packet("Hello2");
        // client->send_reliable_packet("Hello3");
        // client->send_unreliable_packet("Hello4");
        // io_context.run();

        // for how long the client will run:
        std::this_thread::sleep_for(std::chrono::seconds(100000000));
        std::cout << "Client stopped" << std::endl;
        exit(0);
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}