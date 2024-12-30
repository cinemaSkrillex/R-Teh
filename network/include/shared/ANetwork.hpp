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
#include "PacketUtils.hpp"

template <std::size_t BUFFER_SIZE>
class ANetwork : public INetwork {
   protected:
    asio::io_context&                                          _io_context;
    asio::ip::udp::socket                                      _socket;
    PacketManager                                              _packet_manager;
    asio::executor_work_guard<asio::io_context::executor_type> _work_guard;
    std::thread                                                _io_context_thread;
    std::function<void(const asio::ip::udp::endpoint&)>        _new_client_callback;

   public:
    ANetwork(asio::io_context& io_context, unsigned short port, Role role)
        : _io_context(io_context),
          _socket(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)),
          _packet_manager(io_context, _socket, role),
          _work_guard(asio::make_work_guard(io_context)) {
        _packet_manager.start();
        _io_context_thread = std::thread([this]() { _io_context.run(); });
    }

    virtual ~ANetwork() {
        _io_context.stop();
        if (_io_context_thread.joinable()) {
            _io_context_thread.join();
        }
    }

    // callbacks
    void setNewClientCallback(const std::function<void(const asio::ip::udp::endpoint&)>& callback) {
        _new_client_callback                 = callback;
        _packet_manager._new_client_callback = callback;
    }

    // Send methods
    void send_unreliable_packet(const std::array<char, BUFFER_SIZE>& message,
                                const asio::ip::udp::endpoint&       endpoint) override {
        _packet_manager.send_unreliable_packet(message, endpoint);
    }

    void send_reliable_packet(const std::array<char, BUFFER_SIZE>& message,
                              const asio::ip::udp::endpoint&       endpoint) override {
        _packet_manager.send_reliable_packet(message, endpoint);
    }

    // Receive methods
    std::vector<std::array<char, BUFFER_SIZE>> get_unreliable_messages_from_endpoint(
        const asio::ip::udp::endpoint& endpoint) override {
        return _packet_manager.get_unreliable_messages_from_endpoint_data(endpoint);
    }

    std::vector<std::array<char, BUFFER_SIZE>> get_reliable_messages_from_endpoint(
        const asio::ip::udp::endpoint& endpoint) override {
        return _packet_manager.get_reliable_messages_from_endpoint_data(endpoint);
    }

    std::array<char, BUFFER_SIZE> get_last_unreliable_packet_data() override {
        return _packet_manager.get_last_unreliable_packet_data();
    }

    std::array<char, BUFFER_SIZE> get_last_reliable_packet_data() override {
        return _packet_manager.get_last_reliable_packet_data();
    }

    // miscalleneous
    void send_new_client(const asio::ip::udp::endpoint& endpoint) {
        _packet_manager.send_new_client(endpoint);
    }
};

#endif /* !ANETWORK_HPP_ */
