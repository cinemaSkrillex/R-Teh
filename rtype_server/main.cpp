/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** main
*/

#include <SFML/System/Clock.hpp>
#include <asio.hpp>
#include <iostream>
#include <thread>

#include "include/Server/UDPServer.hpp"

#if defined(_WIN32) || defined(_WIN64)
#define _WIN32_WINNT 0x0A00
#endif

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

const sf::Int32 SERVER_TICK = 10;

// Example callback function
void on_new_client() {
    std::cout << "callback client" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    unsigned short port = static_cast<unsigned short>(std::stoi(argv[1]));

    try {
        asio::io_context io_context;
        auto             server = std::make_shared<UDPServer>(io_context, port);

        // Run io_context in a separate thread
        std::thread io_thread([&io_context]() { io_context.run(); });

        server->setNewClientCallback(on_new_client);

        sf::Clock tickClock;
        while (true) {
            if (tickClock.getElapsedTime().asMilliseconds() > 1000 / SERVER_TICK) {
                // reset the clock for next tick.
                tickClock.restart();

                // do server work.
                const std::string message = server->getLastUnreliablePacket();
                if (!message.empty()) std::cout << "Server tick: " << message << std::endl;
                for (auto client : server->getClients()) {
                    server->send_unreliable_packet("tick\n", client);
                }
            }
        }

        io_thread.join();  // Wait for the io_thread to finish (if needed)
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
