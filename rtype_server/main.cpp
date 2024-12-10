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

void setPlayerLastTimestamp(long int uuid, long int timestamp) {
    for (int i = 0; i < PLAYERS.size(); i++) {
        if (PLAYERS.at(i).getUUID() == uuid) {
            PLAYERS.at(i).setLastTimestamp(timestamp);
            return;
        }
    }
}

long int getPlayerLastTimestamp(long int uuid) {
    for (int i = 0; i < PLAYERS.size(); i++) {
        if (PLAYERS.at(i).getUUID() == uuid) {
            return PLAYERS.at(i).getLastTimestamp();
        }
    }
    return -1;
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
        float            deltaTime     = 0.f;
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
            long elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(
                                    std::chrono::steady_clock::now() - start_time)
                                    .count();
            auto player =
                Player(uuid, elapsed_time, game_instance->addPlayer(uuid, PLAYER_START_POSITION),
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
                    for (const auto& message :
                         server->get_unreliable_messages_from_endpoint(client)) {
                        std::cout << "Message: " << message << std::endl;

                        const auto parsed_data      = parse_message(message);
                        const auto player_direction = parseDirection(parsed_data.at("Direction"));
                        const auto player_uuid      = std::stol(parsed_data.at("Uuid"));
                        const auto timestamp        = std::stol(parsed_data.at("Timestamp"));

                        const auto lastTimestamp       = getPlayerLastTimestamp(player_uuid);
                        long       client_elapsed_time = timestamp - lastTimestamp;
                        float      client_elapsed_time_seconds = client_elapsed_time / 1000.f;

                        setPlayerLastTimestamp(player_uuid, timestamp);

                        std::cout << "Player UUID: " << player_uuid << std::endl;
                        std::cout << "Player Direction: " << player_direction.x << ", "
                                  << player_direction.y << std::endl;
                        std::cout << "Player Timestamp: " << timestamp << std::endl;
                        std::cout << "Player Last Timestamp: " << lastTimestamp << std::endl;
                        std::cout << "Client Elapsed Time (ms): " << client_elapsed_time
                                  << std::endl;

                        // Simulate multiple steps if necessary (for multiple frames per server
                        // tick)
                        const float server_tick_duration =
                            1.0f / 10;  // e.g., 0.1f for 10 ticks per second
                        float time_to_simulate = client_elapsed_time_seconds;

                        // Simulate the physics, splitting the client elapsed time if necessary
                        while (time_to_simulate > 0.0f) {
                            float delta_time = std::min(time_to_simulate, server_tick_duration);
                            game_instance->movePlayer(player_uuid, player_direction, delta_time);
                            game_instance->run(delta_time);

                            // Decrease the time remaining to simulate
                            time_to_simulate -= delta_time;
                        }
                    }
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
