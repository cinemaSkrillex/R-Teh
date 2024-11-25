/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** TestingClientMain.cpp
*/
#include <iostream>
#include <asio.hpp>
#include "include/DynamicLibrary/DynamicLibrary.hpp"
#include "include/Client/UDPClient.hpp"

#if defined(_WIN32) || defined(_WIN64)
#define _WIN32_WINNT 0x0A00
#endif

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <dlfcn.h>
#endif

typedef UDPClient* (*UDPClientConstructor)(asio::io_context&, unsigned short);

class TestClient {
  public:
    TestClient(asio::io_context& io_context, const std::string& server_ip,
               unsigned short server_port, unsigned short client_port)
        : io_context_(io_context), client_(nullptr),
          server_endpoint_(asio::ip::address::from_string(server_ip), server_port) {
        std::string library_extension;
#if defined(_WIN32) || defined(_WIN64)
        library_extension = ".dll";
#else
        library_extension = ".so";
#endif

        // Construct the library file name
        std::string    library_name = "librtype_client" + library_extension;
        DynamicLibrary lib(library_name);
        auto           create_client = lib.get_symbol<UDPClientConstructor>("create_udp_client");

        client_ = create_client(io_context_, client_port);
    }

    ~TestClient() { delete client_; }

    void start() {
        // Start the client
        io_context_.run();
    }

    void send_test_packets() {
        // Send an unreliable packet to the server
        client_->send_unreliable_packet("Unreliable packet", server_endpoint_);
        client_->send_unreliable_packet("Unreliable packet again", server_endpoint_);
        // std::uint32_t sequence_number = 0;
        // client_->send_ack(sequence_number);
        // Send a reliable packet to the server
        // client_->send_reliable_packet("Reliable packet", server_endpoint_);
    }

  private:
    asio::io_context&       io_context_;
    UDPClient*              client_;
    asio::ip::udp::endpoint server_endpoint_;
};

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
        TestClient       test_client(io_context, server_ip, server_port, client_port);

        // Send test packets to the server
        test_client.send_test_packets();

        // Start the client
        test_client.start();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}