#include <atomic>
#include <iostream>
#include <string>
#include <thread>

#include "../../../../network/include/Client/UDPClient.hpp"

int main(int argc, char* argv[]) {
    // Check and parse command-line arguments
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <server_ip> <server_port> <client_port>"
                  << std::endl;
        return 1;
    }

    std::string    server_ip   = argv[1];
    unsigned short server_port = static_cast<unsigned short>(std::stoi(argv[2]));
    unsigned short client_port = static_cast<unsigned short>(std::stoi(argv[3]));

    try {
        // Initialize ASIO and the UDP client
        asio::io_context io_context;
        auto client = std::make_shared<UDPClient>(io_context, client_port, server_ip, server_port);

        std::atomic<bool> running(true);

        // Launch the game on the main thread
        // added the port, temporarily for testing.

        client->send_new_client();

        // Game loop

        running.store(false);
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
