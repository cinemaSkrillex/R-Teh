/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** main
*/

#include "include/Game/GameInstance.hpp"
#include "include/RtypeServer.hpp"
#include "include/shared/GenerateUuid.hpp"

std::unordered_map<std::string, std::string> parse_message(const std::string& message) {
    std::unordered_map<std::string, std::string> parsed_data;

    // Break the message into key-value pairs using a delimiter
    std::istringstream stream(message);
    std::string        token;

    // Process each key-value pair
    while (std::getline(stream, token, ' ')) {  // ' ' as delimiter for key:value pairs
        auto delimiter_pos = token.find(':');
        if (delimiter_pos != std::string::npos) {
            std::string key   = token.substr(0, delimiter_pos);
            std::string value = token.substr(delimiter_pos + 1);
            parsed_data[key]  = value;  // Store in the map
        }
    }

    return parsed_data;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    unsigned short port = static_cast<unsigned short>(std::stoi(argv[1]));

    try {
        asio::io_context io_context;
        auto             server = std::make_shared<UDPServer>(io_context, port);

        // Run io_context in a separate thread
        std::thread io_thread([&io_context]() { io_context.run(); });

        server->setNewClientCallback([server](const asio::ip::udp::endpoint& new_client) {
            std::cout << "Callback: New client connected from " << new_client.address()
                      << std::endl;
            std::vector<int> players = {};

            UUIDGenerator uuid_generator;
            long int      uuid = uuid_generator.generate_long();
            std::cout << "Generated UUID: " << uuid << std::endl;
            // Notify all other clients about the new client
            for (const auto& client : server->getClients()) {
                if (client != new_client) {
                    const std::string message =
                        "Event:New_client Uuid:" + std::to_string(new_client.port()) +
                        " Position:(20,60)";
                    server->send_reliable_packet(message, client);
                    players.push_back(client.port());
                }
            }
            // Create the uuid for each new client
            std::string message =
                "Event:Synchronize Uuid:" + std::to_string(new_client.port()) + " Players:[";
            for (int i = 0; i < players.size(); i++) {
                if (i != 0) message += "|";
                message += std::to_string(players.at(i)) + ",(100.0,100.0)";
            }
            message += "]";
            server->send_reliable_packet(message, new_client);
        });

        sf::Clock tickClock;
        while (true) {
            if (tickClock.getElapsedTime().asMilliseconds() > 1000 / SERVER_TICK) {
                // reset the clock for next tick.
                tickClock.restart();

                // do server work.
                for (auto client : server->getClients()) {
                    std::cout << "Parsing Client: " << client.port() << std::endl;
                    for (const auto messages :
                         server->get_unreliable_messages_from_endpoint(client)) {
                        const auto parsed_data = parse_message(messages);
                        if (parsed_data.find("Direction") != parsed_data.end()) {
                            std::cout << "Direction: " << parsed_data.at("Direction") << std::endl;
                        }
                    }
                    // server->send_unreliable_packet("tick\n", client);
                }
            }
        }

        io_thread.join();  // Wait for the io_thread to finish (if needed)
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
