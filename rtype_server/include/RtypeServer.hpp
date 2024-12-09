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

#include "Game/GameInstance.hpp"
#include "Server/UDPServer.hpp"

#if defined(_WIN32) || defined(_WIN64)
#define _WIN32_WINNT 0x0A00
#endif

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

const sf::Int32    SERVER_TICK           = 10;
const sf::Vector2f PLAYER_START_POSITION = {50.f, 250.f};

class Player {
   public:
    long int     getUUID() const { return _uuid; }
    sf::Vector2f getPosition() const {
        RealEngine::Position* component = _registry->get_component<RealEngine::Position>(*_entity);
        return { component->x, component->y };
    }
    const std::vector<sf::Vector2f>& getPositions() const { return _positions; }

   private:
    long int                  _uuid;
    std::vector<sf::Vector2f> _positions;
    RealEngine::Entity*       _entity;
    RealEngine::Registry*     _registry;

   public:
    Player(/* args */);
    Player(long int uuid, RealEngine::Entity* player_entity, RealEngine::Registry* registry);
    ~Player();
};

static std::vector<Player> PLAYERS = {};

class RtypeServer {
   private:
    std::unordered_map<int, Player> _players;

   public:
    RtypeServer(/* args */);
    ~RtypeServer();
};
