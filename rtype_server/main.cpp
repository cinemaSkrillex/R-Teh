/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** main
*/

#include <regex>

#include "GenerateUuid.hpp"
#include "RtypeServer.hpp"

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

sf::Vector2f parseDirection(const std::string& directionStr) {
    sf::Vector2f direction(0.f, 0.f);  // Default to (0, 0) in case of a parsing error

    // Regex to match floating-point numbers inside parentheses
    std::regex  directionRegex(R"(\(([-+]?\d*\.?\d+),([-+]?\d*\.?\d+)\))");
    std::smatch match;

    if (std::regex_search(directionStr, match, directionRegex)) {
        // Convert the extracted strings to floats
        direction.x = std::stof(match[1].str());
        direction.y = std::stof(match[2].str());
    } else {
        std::cerr << "Failed to parse direction: " << directionStr << std::endl;
    }

    return direction;
}

std::string formatTimestamp(const std::chrono::steady_clock::time_point& start_time) {
    auto now     = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count();
    return std::to_string(elapsed);  // in milliseconds
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    unsigned short port = static_cast<unsigned short>(std::stoi(argv[1]));

    try {
        asio::io_context io_context;
        auto             server        = std::make_shared<UDPServer>(io_context, port);
        auto             game_instance = std::make_shared<GameInstance>();
        std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();

        // Run io_context in a separate thread
        std::thread io_thread([&io_context]() { io_context.run(); });

        server->setNewClientCallback([server, game_instance,
                                      start_time](const asio::ip::udp::endpoint& new_client) {
            std::cout << "Callback: New client connected from " << new_client.address()
                      << std::endl;

            UUIDGenerator uuid_generator;
            long int      uuid = uuid_generator.generate_long();
            std::cout << "Generated UUID: " << uuid << std::endl;
            // Notify all other clients about the new client
            for (const auto& client : server->getClients()) {
                if (client != new_client) {
                    const std::string message = "Event:New_client Uuid:" + std::to_string(uuid) +
                                                " Position:(" +
                                                std::to_string(PLAYER_START_POSITION.x) + "," +
                                                std::to_string(PLAYER_START_POSITION.y) + ")";

                    server->send_reliable_packet(message, client);
                }
            }
            // Create the uuid for each new client
            std::string message = "Event:Synchronize Uuid:" + std::to_string(uuid) +
                                  " Clock:" + formatTimestamp(start_time) + " Position:(" +
                                  std::to_string(PLAYER_START_POSITION.x) + "," +
                                  std::to_string(PLAYER_START_POSITION.y) + ") Players:[";
            for (int i = 0; i < PLAYERS.size(); i++) {
                if (i != 0) message += "|";
                message += std::to_string(PLAYERS.at(i).getUUID()) + ",(" +
                           std::to_string(PLAYERS.at(i).getPosition().x) + "," +
                           std::to_string(PLAYERS.at(i).getPosition().y) + ")";
            }
            message += "]";
            server->send_reliable_packet(message, new_client);
            auto player = Player(uuid, game_instance->addPlayer(uuid, PLAYER_START_POSITION),
                                 game_instance->getRegistry());
            PLAYERS.push_back(player);
        });

        sf::Clock tickClock;
        while (true) {
            if (tickClock.getElapsedTime().asMilliseconds() > 1000 / SERVER_TICK) {
                // reset the clock for next tick.
                deltaTime = tickClock.restart().asSeconds();

                // do server work.
                for (auto client : server->getClients()) {
                    std::cout << "Parsing Client: " << client.port() << std::endl;
                    for (const auto messages :
                         server->get_unreliable_messages_from_endpoint(client)) {
                        std::cout << "Message: " << messages << std::endl;
                        const auto parsed_data      = parse_message(messages);
                        const auto player_direction = parseDirection(parsed_data.at("Direction"));
                        const auto player_uuid      = std::stol(parsed_data.at("Uuid"));

                        game_instance->movePlayer(player_uuid, player_direction, deltaTime * messages.size());
                        game_instance->run(deltaTime * messages.size());
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
