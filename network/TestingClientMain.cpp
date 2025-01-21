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
        std::cerr << "Usage: " << argv[0] << " <serverIp> <serverPort>" << std::endl;
        return 1;
    }

    std::string    serverIp   = argv[1];
    unsigned short serverPort = static_cast<unsigned short>(std::stoi(argv[2]));
    // unsigned short clientPort = static_cast<unsigned short>(std::stoi(argv[3]));

    try {
        std::cout << "Starting client" << std::endl;
        asio::io_context ioContext;

        auto tcpclient = std::make_shared<TCPClient>(serverIp, serverPort);

        // auto client = std::make_shared<UDPClient>(ioContext, serverIp, serverPort);
        // client->sendNewClient();
        // client->send_test();
        // client->sendUnreliablePacket("Hello");
        // client->sendUnreliablePacket("World");
        // client->sendReliablePacket("Hello1");
        // client->sendUnreliablePacket("Hello2");
        // client->sendReliablePacket("Hello3");
        // client->sendUnreliablePacket("Hello4");
        // ioContext.run();

        // for how long the client will run:
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Client stopped" << std::endl;
        exit(0);
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}