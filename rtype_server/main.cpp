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

    std::cout << "Starting server on port " << port << std::endl;

    try {
        asio::io_context             io_context;
        std::cout << "IO context created" << std::endl;
        auto                         server       = std::make_shared<UDPServer>(io_context, port);
        std::cout << "Server created" << std::endl;
        std::shared_ptr<RtypeServer> rtype_server = std::make_shared<RtypeServer>(server);
        std::cout << "RtypeServer created" << std::endl;
        std::chrono::steady_clock::time_point start_time = rtype_server->getStartTime();

        std::cout << "Server started at " << start_time.time_since_epoch().count() << std::endl;

        rtype_server->run();

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
