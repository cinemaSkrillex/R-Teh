/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** main.cpp
*/
#include <iostream>
#include <dlfcn.h>
#include <asio.hpp>
#include "include/DynamicLibrary/DynamicLIbrary.hpp"

typedef void (*UDPServerConstructor)(asio::io_context&, unsigned short);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    unsigned short port = static_cast<unsigned short>(std::stoi(argv[1]));

    try {
        std::string library_extension;
#if defined(_WIN32) || defined(_WIN64)
        library_extension = ".dll";
#else
        library_extension = ".so";
#endif

        // Construct the library file name
        std::string    library_name = "librtype_server" + library_extension;
        DynamicLibrary lib(library_name);
        auto           create_server =
            lib.get_symbol<UDPServerConstructor* (*)(asio::io_context&, unsigned short)>(
                "create_udp_server");

        asio::io_context      io_context;
        UDPServerConstructor* server = create_server(io_context, port);
        io_context.run();
        delete server; // Clean up the server instance
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}