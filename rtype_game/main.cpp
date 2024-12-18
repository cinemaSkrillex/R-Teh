#include <atomic>
#include <thread>

#include "Game.hpp"

std::vector<char> serialize_int_vector(const std::vector<int>& int_vector) {
    std::vector<char> buffer;
    buffer.reserve(int_vector.size() * sizeof(int));

    for (int value : int_vector) {
        char data[sizeof(int)];
        std::memcpy(data, &value, sizeof(int));
        buffer.insert(buffer.end(), data, data + sizeof(int));
    }

    return buffer;
}

void print_serialized_data(const std::vector<char>& data) {
    std::cout << "Serialized data: ";
    for (unsigned char byte : data) {
        std::cout << std::hex << static_cast<unsigned int>(byte) << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    // Check and parse command-line arguments
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <server_ip> <server_port> <client_port>"
                  << std::endl;
        return 1;
    }

    std::string    server_ip   = argv[1];
    unsigned short server_port = static_cast<unsigned short>(std::stoi(argv[2]));
    unsigned short client_port = static_cast<unsigned short>(std::stoi(argv[3]));

    try {
        // Initialize ASIO and the UDP client
        asio::io_context io_context;
        auto client = std::make_shared<UDPClient>(io_context, client_port, server_ip, server_port);

        std::atomic<bool> running(true);

        // Launch the game on the main thread
        // added the port, temporarily for testing.
        rtype::Game game(client, client_port);

        client->send_new_client();

        std::vector<int>  int_vector      = {100000, 25848, 3, 4, 5};
        std::vector<char> serialized_data = serialize_int_vector(int_vector);

        std::cout << "Sending data to server" << std::endl;
        std::cout << "Data: ";
        for (int value : int_vector) {
            std::cout << value << " ";
        }
        std::cout << std::endl;

        print_serialized_data(serialized_data);

        // Send the serialized data
        client->send_unreliable_packet(serialized_data);
        // Game loop
        std::cout << "Starting game loop" << std::endl;

        game.run();

        running.store(false);
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
