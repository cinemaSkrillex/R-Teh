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

#include "UDPServer.hpp"
#include "include/Server/UDPServer.hpp"

#if defined(_WIN32) || defined(_WIN64)
#define _WIN32_WINNT 0x0A00
#endif

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

const sf::Int32 SERVER_TICK = 10;

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

        server->setNewClientCallback([server](const asio::ip::udp::endpoint& new_client) {
            std::cout << "Callback: New client connected from " << new_client.address() << std::endl;
            std::vector<int> players = {};

            // Notify all other clients about the new client
            for (const auto& client : server->getClients()) {
                if (client != new_client) {
                    const std::string message =
                        "Event:New_client Uuid:" + std::to_string(new_client.port()) + " Position:(20,60)";
                    server->send_reliable_packet(message, client);
                    players.push_back(client.port());
                }
            }

            std::string message = "Event:Synchronize Uuid:" + std::to_string(new_client.port()) + " Players:[";
            for (int i = 0; i < players.size(); i++) {
                if (i!=0)
                    message += "|";
                message += std::to_string(players.at(i)) + ",(100.0,100.0)";
            }
            message +="]";
            server->send_reliable_packet(message, new_client);
        });

        sf::Clock tickClock;
        while (true) {
            if (tickClock.getElapsedTime().asMilliseconds() > 1000 / SERVER_TICK) {
                // reset the clock for next tick.
                tickClock.restart();

                // do server work.
                const std::string message = server->getLastUnreliablePacket();
                if (!message.empty()) std::cout << "Server tick: " << message << std::endl;
                for (auto client : server->getClients()) {
                    // server->send_unreliable_packet("tick\n", client);
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
