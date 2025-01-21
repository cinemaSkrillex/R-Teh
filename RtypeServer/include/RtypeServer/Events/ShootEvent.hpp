/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** ShootEvent
*/

#pragma once

#include <array>

#include "AEvent.hpp"

class RtypeServer;
class ServerPlayer;
#include <asio.hpp>

class ShootEvent : public AEvent {
   public:
    void execute(const std::array<char, 800>& buffer, const asio::ip::udp::endpoint& client,
                 ServerPlayer& player, RtypeServer* server);
};
