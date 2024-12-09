/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RtypeServer
*/

#pragma once

#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
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
const sf::Vector2f PLAYER_START_POSITION = {50.f, 50.f};

class Player
{
private:
    sf::Vector2f _position;
    std::vector<sf::Vector2f> _positions;

public:
    Player(/* args */);
    Player(sf::Vector2f current_position);
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

