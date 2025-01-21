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
    asio::io_context&                                          _ioContext;
    asio::ip::udp::socket                                      _socket;
    PacketManager<BUFFER_SIZE>                                 _packetManager;
    asio::executor_work_guard<asio::io_context::executor_type> _workGuard;
    std::thread                                                _ioContextThread;
    std::function<void(const asio::ip::udp::endpoint&)>        _newClientCallback;

   public:
    ANetwork(asio::io_context& ioContext, unsigned short port, Role role)
        : _ioContext(ioContext),
          _socket(ioContext, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)),
          _packetManager(ioContext, _socket, role),
          _workGuard(asio::make_work_guard(ioContext)) {
        _packetManager.start();
        _ioContextThread = std::thread([this]() { _ioContext.run(); });
    }

    virtual ~ANetwork() {
        _ioContext.stop();
        if (_ioContextThread.joinable()) {
            _ioContextThread.join();
        }
    }

    asio::ip::udp::endpoint getLocalEndpoint() const { return _socket.local_endpoint(); }

    // callbacks
    void setNewClientCallback(const std::function<void(const asio::ip::udp::endpoint&)>& callback) {
        _newClientCallback                = callback;
        _packetManager._newClientCallback = callback;
    }

    // Send methods
    void sendUnreliablePacket(const std::array<char, BUFFER_SIZE>& message,
                              const asio::ip::udp::endpoint&       endpoint) {
        _packetManager.sendUnreliablePacket(message, endpoint);
    }

    void sendReliablePacket(const std::array<char, BUFFER_SIZE>& message,
                            const asio::ip::udp::endpoint&       endpoint) {
        _packetManager.sendReliablePacket(message, endpoint);
    }

    // Receive methods
    std::vector<std::array<char, BUFFER_SIZE>> getUnreliableMessagesFromEndpoint(
        const asio::ip::udp::endpoint& endpoint) {
        return _packetManager.getUnreliableMessagesFromEndpointData(endpoint);
    }

    std::vector<std::array<char, BUFFER_SIZE>> getReliableMessagesFromEndpoint(
        const asio::ip::udp::endpoint& endpoint) {
        return _packetManager.getReliableMessagesFromEndpointData(endpoint);
    }

    std::array<char, BUFFER_SIZE> getLastUnreliablePacketData() {
        return _packetManager.getLastUnreliablePacketData();
    }

    std::array<char, BUFFER_SIZE> getLastReliablePacketData() {
        return _packetManager.getLastReliablePacketData();
    }

    // Miscellaneous
    void sendNewClient(const asio::ip::udp::endpoint& endpoint) {
        _packetManager.sendNewClient(endpoint);
    }

    std::unordered_set<asio::ip::udp::endpoint, EndpointHash, EndpointEqual> getClients() {
        return _packetManager.getKnownClients();
    }
};

#endif /* !ANETWORK_HPP_ */
