#include <atomic>
#include <thread>

#include "Game/Game.hpp"
#include "Game/GameMenu.hpp"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <server_ip> <server_port> <client_port>"
                  << std::endl;
        return 1;
    }

    std::string    server_ip   = argv[1];
    unsigned short server_port = static_cast<unsigned short>(std::stoi(argv[2]));
    unsigned short client_port = static_cast<unsigned short>(std::stoi(argv[3]));

    try {
        asio::io_context io_context;
        auto client = std::make_shared<UDPClient>(io_context, client_port, server_ip, server_port);
        std::atomic<bool> running(true);
        rtype::Game       game(client, client_port);
        // client->send_new_client();
        game.run();
        running.store(false);
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
