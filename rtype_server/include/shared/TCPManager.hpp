/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** TCPManager.hpp
*/

#pragma once

#include <asio.hpp>
#include <iostream>
#include <memory>
#include <vector>

#include "PacketUtils.hpp"

// Structure for the TCP binary protocol
struct TCPPacket {
    Flags             flag;
    std::vector<char> data;

    bool operator==(const TCPPacket& other) const {
        return flag == other.flag && data == other.data;
    }
};

std::vector<char> serializePacket(const TCPPacket& pkt) {
    std::vector<char> buffer;
    // Add flag
    buffer.push_back(static_cast<char>(pkt.flag));
    // Add data
    buffer.insert(buffer.end(), pkt.data.begin(), pkt.data.end());
    return buffer;
}

TCPPacket deserializePacket(const std::vector<char>& buffer) {
    TCPPacket pkt;
    // Extract flag
    pkt.flag = static_cast<Flags>(buffer[0]);
    // Extract data
    pkt.data.assign(buffer.begin() + 1, buffer.end());
    return pkt;
}

class TCPManager {
   public:
    TCPManager(asio::io_context& io_context, const std::string& ip, const std::string& port)
        : m_socket(io_context), m_resolver(io_context) {}

    void read(std::vector<char>& buffer) { asio::read(m_socket, asio::buffer(buffer)); }

   private:
    asio::ip::tcp::socket   m_socket;
    asio::ip::tcp::resolver m_resolver;
};