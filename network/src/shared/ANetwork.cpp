/*
** EPITECH PROJECT, 2024
** r-teh-mega-backup
** File description:
** ANetwork
*/

#include "../../include/shared/ANetwork.hpp"

template <std::size_t BUFFER_SIZE>
ANetwork<BUFFER_SIZE>::ANetwork(asio::io_context& io_context, unsigned short port, Role role)
    : _io_context(io_context),
      _socket(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)),
      _packet_manager(io_context, _socket, role),
      _work_guard(asio::make_work_guard(io_context)) {
    _packet_manager.start();
    _io_context_thread = std::thread([this]() { _io_context.run(); });
}

template <std::size_t BUFFER_SIZE>
ANetwork<BUFFER_SIZE>::~ANetwork() {
    _io_context.stop();
    if (_io_context_thread.joinable()) {
        _io_context_thread.join();
    }
}

// callbacks
template <std::size_t BUFFER_SIZE>
void ANetwork<BUFFER_SIZE>::setNewClientCallback(
    const std::function<void(const asio::ip::udp::endpoint&)>& callback) {
    _new_client_callback                 = callback;
    _packet_manager._new_client_callback = callback;
}

// Send methods
template <std::size_t BUFFER_SIZE>
void ANetwork<BUFFER_SIZE>::send_unreliable_packet(const std::array<char, BUFFER_SIZE>& message,
                                                   const asio::ip::udp::endpoint&       endpoint) {
    _packet_manager.send_unreliable_packet(message, endpoint);
}

template <std::size_t BUFFER_SIZE>
void ANetwork<BUFFER_SIZE>::send_reliable_packet(const std::array<char, BUFFER_SIZE>& message,
                                                 const asio::ip::udp::endpoint&       endpoint) {
    _packet_manager.send_reliable_packet(message, endpoint);
}

// Receive methods
template <std::size_t BUFFER_SIZE>
std::vector<std::array<char, BUFFER_SIZE>>
ANetwork<BUFFER_SIZE>::get_unreliable_messages_from_endpoint(
    const asio::ip::udp::endpoint& endpoint) {
    return _packet_manager.get_unreliable_messages_from_endpoint_data(endpoint);
}

template <std::size_t BUFFER_SIZE>
std::vector<std::array<char, BUFFER_SIZE>>
ANetwork<BUFFER_SIZE>::get_reliable_messages_from_endpoint(
    const asio::ip::udp::endpoint& endpoint) {
    return _packet_manager.get_reliable_messages_from_endpoint_data(endpoint);
}

template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> ANetwork<BUFFER_SIZE>::get_last_unreliable_packet_data() {
    return _packet_manager.get_last_unreliable_packet_data();
}

template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> ANetwork<BUFFER_SIZE>::get_last_reliable_packet_data() {
    return _packet_manager.get_last_reliable_packet_data();
}

// miscalleneous
template <std::size_t BUFFER_SIZE>
void ANetwork<BUFFER_SIZE>::send_new_client(const asio::ip::udp::endpoint& endpoint) {
    _packet_manager.send_new_client(endpoint);
}

template <std::size_t BUFFER_SIZE>
std::unordered_set<asio::ip::udp::endpoint, EndpointHash, EndpointEqual>
ANetwork<BUFFER_SIZE>::getClients() {
    return _packet_manager.getKnownClients();
}