/*
** EPITECH PROJECT, 2024
** r-teh-mega-backup
** File description:
** ANetwork
*/
#ifndef ANETWORK_HPP_
#define ANETWORK_HPP_

#include <array>
#include <asio.hpp>
#include <thread>
#include <vector>

#include "INetwork.hpp"
#include "PacketManager.hpp"
#include "PacketUtils.hpp"

template <std::size_t BUFFER_SIZE>
class ANetwork : public INetwork<BUFFER_SIZE> {
   protected:
    asio::io_context&                                          _io_context;
    asio::ip::udp::socket                                      _socket;
    PacketManager<BUFFER_SIZE>                                 _packet_manager;
    asio::executor_work_guard<asio::io_context::executor_type> _work_guard;
    std::thread                                                _io_context_thread;
    std::function<void(const asio::ip::udp::endpoint&)>        _new_client_callback;

   public:
    ANetwork(asio::io_context& io_context, unsigned short port, Role role);

    virtual ~ANetwork();
    // callbacks
    void setNewClientCallback(const std::function<void(const asio::ip::udp::endpoint&)>& callback);
    // Send methods
    void send_unreliable_packet(const std::array<char, BUFFER_SIZE>& message,
                                const asio::ip::udp::endpoint&       endpoint);

    void send_reliable_packet(const std::array<char, BUFFER_SIZE>& message,
                              const asio::ip::udp::endpoint&       endpoint);

    // Receive methods
    std::vector<std::array<char, BUFFER_SIZE>> get_unreliable_messages_from_endpoint(
        const asio::ip::udp::endpoint& endpoint);

    std::vector<std::array<char, BUFFER_SIZE>> get_reliable_messages_from_endpoint(
        const asio::ip::udp::endpoint& endpoint);
    std::array<char, BUFFER_SIZE> get_last_unreliable_packet_data();
    std::array<char, BUFFER_SIZE> get_last_reliable_packet_data();
    // miscalleneous
    void send_new_client(const asio::ip::udp::endpoint& endpoint);
    std::unordered_set<asio::ip::udp::endpoint, EndpointHash, EndpointEqual> getClients();
};

#endif /* !ANETWORK_HPP_ */
