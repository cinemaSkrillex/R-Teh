#include <atomic>
#include <thread>

#include "Game/Game.hpp"

// void create_game() {
//     rtype::Game game(client, clientPort);  // Local object
//     game.run();  // If the object is destroyed here, this will cause undefined behavior.
// }  // game is destroyed here.

int main(int argc, char* argv[]) {
    std::string    serverIp;
    unsigned short serverPort;
    unsigned short clientPort;

    if (LaunchGameWithoutArgs == true) {
        rtype::LaunchGame launcher;
        launcher.run();

        serverIp   = launcher.getServerIp();
        serverPort = launcher.getServerPort();
        clientPort = launcher.getClientPort();

    } else {
        if (argc != 4) {
            std::cerr << "Usage: " << argv[0] << " <serverIp> <serverPort> <clientPort>"
                      << std::endl;
            exit(1);
        }

        serverIp   = argv[1];
        serverPort = static_cast<unsigned short>(std::stoi(argv[2]));
        clientPort = static_cast<unsigned short>(std::stoi(argv[3]));
    }

    try {
        // Initialize ASIO and the UDP client
        asio::io_context ioContext;
        auto client = std::make_shared<UDPClient>(ioContext, clientPort, serverIp, serverPort);

        std::atomic<bool> running(true);

        // Launch the game on the main thread
        // added the port, temporarily for testing.
        // rtype::Game game(client, clientPort);
        rtype::Game* game = new rtype::Game(client, clientPort);  // Dynamically allocated
        client->sendNewClient();
        game->run();
        // delete game;  // Proper cleanup after the game loop

        // Game loop

        // game.run();

        running.store(false);
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
