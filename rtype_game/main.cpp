#include <atomic>
#include <thread>

#include "Game/Game.hpp"

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
        asio::io_context io_context;
        auto client = std::make_shared<UDPClient>(io_context, client_port, server_ip, server_port);
        std::atomic<bool> running(true);

        rtype::Game* game = new rtype::Game(client, client_port);
        if (LaunchGameWithoutArgs == true) {
            client->send_new_client();
        }
        game->run();
        running.store(false);
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
