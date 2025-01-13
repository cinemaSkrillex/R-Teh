/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** HoldShootEvent
*/

#ifndef HOLDSHOOTEVENT_HPP_
#define HOLDSHOOTEVENT_HPP_

#include <array>

#include "AEvent.hpp"

class RtypeServer;
class Player;
#include <asio.hpp>

class HoldShootEvent : public AEvent {
   public:
    void execute(const std::array<char, 800>& buffer, const asio::ip::udp::endpoint& client,
                 Player& player, RtypeServer* server);
};

#endif /* !HOLDSHOOTEVENT_HPP_ */
