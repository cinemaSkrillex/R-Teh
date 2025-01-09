/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** TestingClientMain.cpp
*/

#include <asio.hpp>
#include <iostream>
#include <thread>

#include "DynamicLibrary/DynamicLibrary.hpp"
#include "../include/Launcher/Launcher.hpp"

#if defined(_WIN32) || defined(_WIN64)
#define _WIN32_WINNT 0x0A00
#endif

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <dlfcn.h>
#endif

int main() {
    Launcher launcher;
    launcher.run();

    return 0;
}

