#include <atomic>
#include <thread>

#include "Game/Game.hpp"

// void create_game() {
//     rtype::Game game(client, client_port);  // Local object
//     game.run();  // If the object is destroyed here, this will cause undefined behavior.
// }  // game is destroyed here.

int main(int argc, char* argv[]) {
    std::string    server_ip;
    unsigned short server_port;
    unsigned short client_port;

    if (LaunchGameWithoutArgs == true) {
        rtype::LaunchGame launcher;
        launcher.run();

        server_ip   = launcher.getServerIp();
        server_port = launcher.getServerPort();
        client_port = launcher.getClientPort();

    } else {
        if (argc != 4) {
            std::cerr << "Usage: " << argv[0] << " <server_ip> <server_port> <client_port>"
                      << std::endl;
            exit(1);
        }

        server_ip   = argv[1];
        server_port = static_cast<unsigned short>(std::stoi(argv[2]));
        client_port = static_cast<unsigned short>(std::stoi(argv[3]));
    }

    try {
        // Initialize ASIO and the UDP client
        asio::io_context io_context;
        auto client = std::make_shared<UDPClient>(io_context, client_port, server_ip, server_port);
        std::atomic<bool> running(true);

        // Launch the game on the main thread
        // added the port, temporarily for testing.
        // rtype::Game game(client, client_port);
        rtype::Game* game = new rtype::Game(client, client_port);  // Dynamically allocated
        client->send_new_client();
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
