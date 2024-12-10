/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** main
*/

#include <regex>

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

Player* getPlayerByUuid(long int uuid) {
    for (int i = 0; i < PLAYERS.size(); i++) {
        if (PLAYERS.at(i).getUUID() == uuid) {
            return &PLAYERS.at(i);
        }
    }
    return nullptr;
}

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

        // sf::Clock tickClock;
        // while (true) {
        //     if (tickClock.getElapsedTime().asMilliseconds() > 1000 / SERVER_TICK) {
        //         // Reset the clock for the next tick
        //         deltaTime = tickClock.restart().asSeconds();

        //         // Do server work
        //         for (auto client : server->getClients()) {
        //             for (const auto& message :
        //                  server->get_unreliable_messages_from_endpoint(client)) {
        //                 const auto parsed_data      = parse_message(message);
        //                 const auto player_direction =
        //                 parseDirection(parsed_data.at("Direction")); const auto player_uuid =
        //                 std::stol(parsed_data.at("Uuid")); const auto timestamp        =
        //                 std::stol(parsed_data.at("Timestamp")); const auto lastTimestamp    =
        //                 getPlayerLastTimestamp(player_uuid); long       client_elapsed_time =
        //                 timestamp - lastTimestamp; float      client_elapsed_time_seconds =
        //                 client_elapsed_time / 1000.f;

        //                 setPlayerLastTimestamp(player_uuid, timestamp);

        //                 // Use consistent server delta time for simulation
        //                 const float server_tick_duration =
        //                     deltaTime;  // Duration for the current tick
        //                 float time_to_simulate = client_elapsed_time_seconds;

        //                 // Simulate the physics, splitting the client elapsed time if necessary
        //                 while (time_to_simulate > 0.0f) {
        //                     float delta_time = std::min(time_to_simulate, server_tick_duration);
        //                     game_instance->movePlayer(player_uuid, player_direction, delta_time);
        //                     game_instance->run(delta_time);

        //                     // Decrease the time remaining to simulate
        //                     time_to_simulate -= delta_time;
        //                 }
        //             }
        //         }
        //     }
        // }

        io_thread.join();  // Wait for the io_thread to finish (if needed)
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
