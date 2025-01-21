/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** ReleaseShootEvent
*/

#ifndef RELEASESHOOTEVENT_HPP_
#define RELEASESHOOTEVENT_HPP_
#include <array>

#include "AEvent.hpp"

class RtypeServer;
class ServerPlayer;
#include <asio.hpp>

class ReleaseShootEvent : public AEvent {
   public:
    void execute(const std::array<char, 800>& buffer, const asio::ip::udp::endpoint& client,
                 ServerPlayer& player, RtypeServer* server);

    void shootMiddleBullet(const std::array<char, 800>&   buffer,
                           const asio::ip::udp::endpoint& client, ServerPlayer& player,
                           RtypeServer* server);

    void shootBigBullet(const std::array<char, 800>& buffer, const asio::ip::udp::endpoint& client,
                        ServerPlayer& player, RtypeServer* server);
};

#endif /* !RELEASESHOOTEVENT_HPP_ */
