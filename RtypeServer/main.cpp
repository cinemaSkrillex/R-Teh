/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** main
*/

#include "RtypeServer/RtypeServer.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 3) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    unsigned short port          = static_cast<unsigned short>(std::stoi(argv[1]));
    bool           server_vision = false;

    if (argc == 3 && std::string(argv[2]) == "-window") {
        server_vision = true;
    }
    try {
        asio::io_context             ioContext;
        auto                         server = std::make_shared<UDPServer>(ioContext, port);
        std::shared_ptr<RtypeServer> rtype_server =
            std::make_shared<RtypeServer>(server, server_vision);
        std::chrono::steady_clock::time_point start_time = rtype_server->getStartTime();

        rtype_server->run();

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
