/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RtypeServer
*/

#pragma once

#include <SFML/System/Clock.hpp>
#include <asio.hpp>
#include <iostream>
#include <thread>

#include "Server/UDPServer.hpp"

#if defined(_WIN32) || defined(_WIN64)
#define _WIN32_WINNT 0x0A00
#endif

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

const sf::Int32 SERVER_TICK = 10;

class Player
{
private:
    float x = 0.f, y = 0.f;
public:
    Player(/* args */);
    ~Player();
};


class RtypeServer
{
private:
    std::unordered_map<int, Player> _players;
public:
    RtypeServer(/* args */);
    ~RtypeServer();
};

