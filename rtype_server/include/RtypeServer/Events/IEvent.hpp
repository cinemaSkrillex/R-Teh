/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** IEvent
*/

#ifndef IEVENT_H
#define IEVENT_H
#include <array>
#include <asio.hpp>

class RtypeServer;
class ServerPlayer;

class IEvent {
   public:
    virtual ~IEvent()                                               = default;
    virtual void execute(const std::array<char, 800>& buffer, const asio::ip::udp::endpoint& client,
                         ServerPlayer& player, RtypeServer* server) = 0;
};

#endif  // IEVENT_H
