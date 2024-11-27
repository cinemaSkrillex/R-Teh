#include <SFML/Graphics.hpp>
#include <thread>

#include "Game.hpp"
#include "Client/UDPClient.hpp"

int main() {
    rtype::Game game;

    // asio::io_context io_context;
    // UDPClient server(io_context, 7069);

    // std::thread io_thread([&io_context]() { io_context.run(); });

    game.run();
    return 0;
}