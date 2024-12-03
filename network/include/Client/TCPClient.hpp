/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** TCPClient.hpp
*/

#ifndef TCPCLIENT_HPP
#define TCPCLIENT_HPP

// #include "../shared/PacketUtils.hpp"
#include "TCPPacketManager.hpp"
#include "TCPPacketUtils.hpp"

class TCPClient {
   public:
    TCPClient(const std::string& host, unsigned short port);
    ~TCPClient();

    void send_message(const std::string& message);

   private:
    std::shared_ptr<TCPPacketManager> _packet_manager;
};

#endif