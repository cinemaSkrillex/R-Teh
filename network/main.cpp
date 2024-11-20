/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** main.cpp
*/
#include <iostream>
#include <dlfcn.h>
#include <asio.hpp>

typedef void (*UDPServerConstructor)(asio::io_context&, unsigned short);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    unsigned short port = static_cast<unsigned short>(std::stoi(argv[1]));

    void* handle = dlopen("librtype_server.so", RTLD_LAZY);
    if (!handle) {
        std::cerr << "Cannot open library: " << dlerror() << std::endl;
        return 1;
    }

    dlerror(); // Reset errors

    UDPServerConstructor create_server = (UDPServerConstructor)dlsym(handle, "create_udp_server");
    const char*          dlsym_error   = dlerror();
    if (dlsym_error) {
        std::cerr << "Cannot load symbol 'create_udp_server': " << dlsym_error << std::endl;
        dlclose(handle);
        return 1;
    }

    try {
        asio::io_context io_context;
        create_server(io_context, port);
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    dlclose(handle);
    return 0;
}