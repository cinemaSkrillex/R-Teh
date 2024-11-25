/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** main.cpp
*/
#include <iostream>
#include <asio.hpp>
#include "include/DynamicLibrary/DynamicLibrary.hpp"
#include "include/Server/UDPServer.hpp"

#if defined(_WIN32) || defined(_WIN64)
#define _WIN32_WINNT 0x0A00
#endif

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <dlfcn.h>
#endif

typedef UDPServer* (*UDPServerConstructor)(asio::io_context&, unsigned short);

class TestServer {
  public:
    TestServer(asio::io_context& io_context, unsigned short port)
        : io_context_(io_context), server_(nullptr) {
        std::string library_extension;
#if defined(_WIN32) || defined(_WIN64)
        library_extension = ".dll";
#else
        library_extension = ".so";
#endif

        // Construct the library file name
        std::string    library_name = "librtype_server" + library_extension;
        DynamicLibrary lib(library_name);
        auto           create_server = lib.get_symbol<UDPServerConstructor>("create_udp_server");

        server_ = create_server(io_context_, port);
    }

    ~TestServer() {
        std::cout << "deleting server";
        delete server_;
    }

    void start() {
        // Start the server
        io_context_.run();
    }

  private:
    asio::io_context& io_context_;
    UDPServer*        server_;
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    unsigned short port = static_cast<unsigned short>(std::stoi(argv[1]));

    try {
        asio::io_context io_context;
        auto             test_server = std::make_shared<TestServer>(io_context, port);

        test_server->start();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}