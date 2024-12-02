/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** TCPServer.hpp
*/

#ifndef TCPCLIENT_HPP
#define TCPCLIENT_HPP

#include "PacketUtils.hpp"
#include "TCPPacketManager.hpp"
#include "TCPPacketUtils.hpp"

class TCPServer {
   public:
    TCPServer(unsigned short port);

   private:
    TCPPacketManager _packet_manager;
};
#endif