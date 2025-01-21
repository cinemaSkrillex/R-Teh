/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** main.cpp
*/

#include <atomic>
#include <thread>

#include "include/Game/PongGame.hpp"

int main(int argc, char* argv[]) {
    // Check and parse command-line arguments
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <serverIp> <serverPort> <clientPort>" << std::endl;
        return 1;
    }

    std::string    serverIp   = argv[1];
    unsigned short serverPort = static_cast<unsigned short>(std::stoi(argv[2]));
    unsigned short clientPort = static_cast<unsigned short>(std::stoi(argv[3]));

    try {
        // Initialize ASIO and the UDP client
        asio::io_context ioContext;
        auto client = std::make_shared<UDPClient>(ioContext, clientPort, serverIp, serverPort);

        std::atomic<bool> running(true);

        // Launch the game on the main thread
        // added the port, temporarily for testing.
        pong::Game game(client, clientPort);

        client->sendNewClient();

        // Game loop

        game.run();

        running.store(false);
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
