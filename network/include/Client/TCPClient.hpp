/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** TCPClient.hpp
*/

#ifndef TCPCLIENT_HPP
#define TCPCLIENT_HPP

#include "PacketUtils.hpp"
#include "TCPPacketManager.hpp"
#include "TCPPacketUtils.hpp"

class TCPClient {
   public:
    TCPClient(const std::string& host, unsigned short port);

   private:
    TCPPacketManager _packet_manager;
};

#endif