/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** main
*/

#include "RtypeServer.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    unsigned short port = static_cast<unsigned short>(std::stoi(argv[1]));

    try {
        asio::io_context             io_context;
        auto                         server        = std::make_shared<UDPServer>(io_context, port);
        auto                         game_instance = std::make_shared<GameInstance>();
        std::shared_ptr<RtypeServer> rtype_server  = std::make_shared<RtypeServer>(server);
        std::chrono::steady_clock::time_point start_time = rtype_server->getStartTime();
        // Run io_context in a separate thread
        std::thread io_thread([&io_context]() { io_context.run(); });

        rtype_server->run();

        io_thread.join();  // Wait for the io_thread to finish (if needed)
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
