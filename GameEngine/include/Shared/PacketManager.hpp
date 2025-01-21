/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** SendPackets.hpp
*/

#ifndef SENDPACKETS_HPP
#define SENDPACKETS_HPP

#include <array>
#include <asio.hpp>
#include <deque>
#include <iostream>
#include <map>
#include <queue>
#include <stack>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>

#include "PacketUtils.hpp"

// Templates need to have their implementation in the header file see the following link for more
// https://stackoverflow.com/a/1353981
// You can also use .tpp files to store the implementation and include them in the header file

template <std::size_t BUFFER_SIZE>
class PacketManager {
   public:
    PacketManager(asio::io_context& ioContext, asio::ip::udp::socket& socket, Role role)
        : _messageId(0),
          _retransmissionTimer(ioContext),
          _socket(socket),
          _role(role),
          _stopProcessing(false),
          _work_guard(asio::make_work_guard(ioContext)) {}
    ~PacketManager() {
        _stopProcessing = true;
        if (_sendPacketThread.joinable()) _sendPacketThread.join();
        if (_receivePacketThread.joinable()) _receivePacketThread.join();
        if (_retryPacketThread.joinable()) _retryPacketThread.join();
    }

    void start() {
        _receivePacketThread = std::thread(&PacketManager::receive, this);
        _sendPacketThread    = std::thread(&PacketManager::send, this);
        _retryPacketThread   = std::thread(&PacketManager::retry, this);
    }

    // packet functions
    packet<BUFFER_SIZE> build_packet(int sequenceNb, int startSequenceNb, int endSequenceNb,
                                     Flags flag, const asio::ip::udp::endpoint& endpoint,
                                     const std::array<char, BUFFER_SIZE>& message) {
        packet<BUFFER_SIZE> pkt;
        pkt.sequenceNb      = sequenceNb;
        pkt.startSequenceNb = startSequenceNb;
        pkt.endSequenceNb   = endSequenceNb;

        // Calculate packet<BUFFER_SIZE> size, ensuring it doesn't exceed BUFFER_SIZE
        pkt.packetSize = std::min(static_cast<std::size_t>(BUFFER_SIZE), message.size());

        size_t start_idx = sequenceNb * BUFFER_SIZE;
        size_t end_idx   = start_idx + pkt.packetSize;

        if (start_idx > message.size()) {
            std::cerr << "Error: Start index out of range!" << std::endl;
            std::cerr << "Start index: " << start_idx << " Message size: " << message.size()
                      << std::endl;
            return {};  // Return an empty packet<BUFFER_SIZE> or handle the error
        }

        // Clear existing data in pkt.data and copy relevant bytes
        pkt.data.fill(0);  // Ensure pkt.data is zero-initialized
        std::copy(message.begin() + start_idx, message.begin() + std::min(end_idx, message.size()),
                  pkt.data.begin());

        pkt.flag     = flag;
        pkt.endpoint = endpoint;
        return pkt;
    }
    // threads function
    void receive() {
        _socket.async_receive_from(asio::buffer(_recvBuffer), _endpoint,
                                   [this](std::error_code ec, std::size_t bytesRecvd) {
                                       if (!ec && bytesRecvd > 0) {
                                           handleReceive(bytesRecvd);
                                       } else {
                                           if (ec) {
                                               std::cerr << "Receive error: " << ec.message()
                                                         << std::endl;
                                           } else {
                                               std::cerr << "Received 0 bytes" << std::endl;
                                           }
                                       }
                                       receive();
                                   });
    }
    void send() {
        while (!_stopProcessing) {
            std::unique_lock<std::mutex> lock(_sendQueueMutex);
            _sendQueueCv.wait(lock, [this] { return !_sendQueue.empty() || _stopProcessing; });
            while (!_sendQueue.empty()) {
                auto pkt = _sendQueue.front();
                _sendQueue.pop_front();
                _sendQueueSet.erase(pkt);
                lock.unlock();
                sendPacket(pkt);
                lock.lock();
            }
        }
    }

    void retry() {
        while (!_stopProcessing) {
            // 150 ms is the time to wait before retrying to send a packet<BUFFER_SIZE> - Windows
            // may need to disable firewall
            std::this_thread::sleep_for(std::chrono::milliseconds(150));
            {
                std::lock_guard<std::mutex> lock(_retryQueueMutex);
                for (const auto& pkt : _retryQueue) {
                    sendPacket(pkt);
                }
            }
        }
    }

    // receive functions
    void handleReceive(std::size_t bytesRecvd) {
        auto message = std::make_shared<std::array<char, BUFFER_SIZE>>();
        std::copy(_recvBuffer.begin(), _recvBuffer.begin() + bytesRecvd, message->begin());

        try {
            packet<BUFFER_SIZE> pkt = deserializePacket(*message);
            switch (pkt.flag) {
                case ACK:
                    handleAck(pkt);
                    break;
                case RELIABLE:
                    handleReliablePacket(pkt);
                    break;
                case UNRELIABLE:
                    handleUnreliablePacket(pkt.data);
                    break;
                case NEW_CLIENT:
                    handleNewClient(_endpoint);
                    break;
                case TEST:
                    handleTest(_endpoint);
                    break;
                default:
                    std::cerr << "Received unknown packet<BUFFER_SIZE> type: " << pkt.flag
                              << std::endl;
                    break;
            }
        } catch (const std::exception& e) {
            std::cerr << "Failed to deserialize packet: " << e.what() << std::endl;
        }
    }
    // handle messages
    void handleAck(const packet<BUFFER_SIZE>& pkt) {
        AckMessage ackMessage = deserializeAck(pkt.data);

        SEQUENCE_TYPE sequence_start = ackMessage.startSequenceNumber;
        SEQUENCE_TYPE sequenceNumber = ackMessage.sequenceNumber;

        {
            std::lock_guard<std::mutex> lock(_retryQueueMutex);
            for (auto it = _retryQueue.begin(); it != _retryQueue.end();) {
                if (it->sequenceNb == sequenceNumber && it->startSequenceNb == sequence_start) {
                    _retryQueue.erase(it);
                    break;
                }
                ++it;
            }
        }
    }

    void handleReliablePacket(const packet<BUFFER_SIZE>& pkt) {
        bool all_packets_received = false;

        if (pkt.sequenceNb > pkt.endSequenceNb) {
            std::cerr << "Invalid sequence number: " << pkt.sequenceNb << std::endl;
            return;
        }

        {
            std::lock_guard<std::mutex> lock(_receivedPacketsMutex);
            // check that the packet<BUFFER_SIZE> is not already in the map
            std::vector<packet<BUFFER_SIZE>> tempVector = _receivedPackets[pkt.startSequenceNb];
            if (std::find(tempVector.begin(), tempVector.end(), pkt) != tempVector.end()) {
            } else {
                _receivedPackets[pkt.startSequenceNb].push_back(pkt);
            }

            int finalSize    = pkt.endSequenceNb - pkt.startSequenceNb + 1;
            int receivedSize = _receivedPackets[pkt.startSequenceNb].size();

            if (receivedSize == finalSize) {
                all_packets_received = true;
            }
        }

        if (all_packets_received) {
            std::array<char, BUFFER_SIZE> completeData = {};
            size_t                        currentIndex = 0;

            {
                std::lock_guard<std::mutex> lock(_receivedPacketsMutex);
                // WARNING: sort could make the program slower
                std::sort(_receivedPackets[pkt.startSequenceNb].begin(),
                          _receivedPackets[pkt.startSequenceNb].end(),
                          [](const packet<BUFFER_SIZE>& a, const packet<BUFFER_SIZE>& b) {
                              return a.sequenceNb < b.sequenceNb;
                          });

                // for (const auto& packet<BUFFER_SIZE> : _receivedPackets[pkt.startSequenceNb])
                // {
                //     completeData.insert(completeData.end(), packet<BUFFER_SIZE>.data.begin(),
                //     packet<BUFFER_SIZE>.data.end());
                // }
                // _receivedPackets.erase(pkt.startSequenceNb);
                // const std::string message = std::string(completeData.begin(),
                // completeData.end());
                for (const auto& packet : _receivedPackets[pkt.startSequenceNb]) {
                    size_t dataSize = std::min(packet.data.size(), BUFFER_SIZE - currentIndex);
                    std::copy(packet.data.begin(), packet.data.begin() + dataSize,
                              completeData.begin() + currentIndex);
                    currentIndex += dataSize;

                    if (currentIndex >= BUFFER_SIZE) {
                        break;  // Prevent exceeding the fixed size
                    }
                    _receivedPackets.erase(pkt.startSequenceNb);
                }

                std::lock_guard<std::mutex> unprocessed_lock(_unprocessedReliableMessagesDataMutex);
                // Push the complete data and endpoint into the reliable messages vector
                _unprocessedReliableMessagesData.push_back(std::make_pair(completeData, _endpoint));

                // Process the message content
                // std::lock_guard<std::mutex>
                // unprocessed_lock(_unprocessed_reliable_messages_mutex);
                // // For now: we will push with the last _endpoint. Can cause issues in the future
                // DON'T REMOVE
                // _unprocessed_reliable_messages.push_back(std::make_pair(message,
                // _endpoint));
            }
        }
        send_ack(pkt.startSequenceNb, pkt.sequenceNb, _endpoint);
    }

    void handleUnreliablePacket(const std::array<char, BUFFER_SIZE>& message) {
        // Process the message content
        std::lock_guard<std::mutex> lock(_unprocessedUnreliableMessagesDataMutex);
        _unprocessedUnreliableMessagesData.push_back(std::make_pair(message, _endpoint));
    }
    // void handleUnreliablePacket(const std::array<char, BUFFER_SIZE>& message);
    void handleNewClient(const asio::ip::udp::endpoint& client_endpoint) {
        if (_knownClients.find(client_endpoint) != _knownClients.end()) return;

        _knownClients.insert(client_endpoint);
        // here we used to send test message. TODO test flag to trigger the test

        if (_newClientCallback) _newClientCallback(client_endpoint);
    }

    void handleTest(const asio::ip::udp::endpoint& endpoint) {
        // send reliable packet<BUFFER_SIZE> to the client with message testPacketManager()
        // sendReliablePacket(testPacketManager(), endpoint);
    }

    // send functions
    void send_ack(SEQUENCE_TYPE startSequenceNumber, SEQUENCE_TYPE sequenceNumber,
                  const asio::ip::udp::endpoint& endpoint_) {
        AckMessage                    ackMessage = {AckType::ACK, 0, 0};
        std::array<char, BUFFER_SIZE> buffer;

        // Set the AckType based on the role
        if (_role == Role::SERVER) {
            ackMessage.ack_type = AckType::CLIENT_ACK;
        } else if (_role == Role::CLIENT) {
            ackMessage.ack_type = AckType::ACK;
        }

        // Set the sequence numbers
        ackMessage.startSequenceNumber = startSequenceNumber;
        ackMessage.sequenceNumber      = sequenceNumber;

        // Serialize the AckMessage into the buffer
        serialize_ack(ackMessage, buffer);

        // build packet
        packet<BUFFER_SIZE> pkt = build_packet(0, 0, 0, ACK, endpoint_, buffer);

        queuePacketForSending(pkt);
    }

    void queuePacketForSending(const packet<BUFFER_SIZE>& pkt) {
        {
            std::lock_guard<std::mutex> lock(_sendQueueMutex);
            if (_sendQueueSet.find(pkt) == _sendQueueSet.end()) {
                _sendQueue.emplace_back(pkt);
                _sendQueueSet.insert(pkt);
            } else {
            }
        }
        _sendQueueCv.notify_one();
    }

    void sendPacket(const packet<BUFFER_SIZE>& pkt) {
        const auto buffer = std::make_shared<std::vector<char>>(serialize_packet(pkt));
        std::this_thread::sleep_for(std::chrono::nanoseconds(100000));
        _socket.async_send_to(asio::buffer(*buffer), pkt.endpoint,
                              [this, buffer](std::error_code ec, std::size_t bytes_sent) {
                                  if (ec) {
                                      std::cerr << "Send error: " << ec.message()
                                                << " size: " << bytes_sent << std::endl;
                                  }
                              });
    }
    void sendReliablePacket(const std::array<char, BUFFER_SIZE>& message,
                            const asio::ip::udp::endpoint&       endpoint) {
        int totalPackets = (message.size() + BUFFER_SIZE - 1) / BUFFER_SIZE;

        int startSequenceNb;
        int endSequenceNb;

        {
            std::lock_guard<std::mutex> lock(_message_id_mutex);
            startSequenceNb = _messageId;
            endSequenceNb   = _messageId + totalPackets - 1;
        }

        for (int i = 0; i < totalPackets; ++i) {
            packet<BUFFER_SIZE> pkt;

            {
                std::lock_guard<std::mutex> lock(_message_id_mutex);
                pkt = build_packet(_messageId - startSequenceNb, startSequenceNb, endSequenceNb,
                                   RELIABLE, endpoint, message);
                _messageId++;
            }

            if (endpoint.address().is_unspecified()) {
                std::cerr << "Server endpoint is unspecified" << std::endl;
                return;
            }
            if (pkt.sequenceNb > pkt.endSequenceNb) {
                std::cerr << "sending reliable Invalid sequence number: " << pkt.sequenceNb
                          << " startSequenceNb: " << pkt.startSequenceNb
                          << " endSequenceNb: " << pkt.endSequenceNb << std::endl;
                continue;
            }
            queuePacketForSending(pkt);
            queuePacketForRetry(pkt);
        }
    }
    void sendNewClient(const asio::ip::udp::endpoint& endpoint) {
        packet<BUFFER_SIZE> pkt = build_packet(0, 0, 0, NEW_CLIENT, endpoint, {0});
        queuePacketForSending(pkt);
    }
    void send_test(const asio::ip::udp::endpoint& endpoint) {
        packet<BUFFER_SIZE> pkt = build_packet(0, 0, 0, TEST, endpoint, {0});
        queuePacketForSending(pkt);
    }

    void sendUnreliablePacket(const std::array<char, BUFFER_SIZE>& message,
                              const asio::ip::udp::endpoint&       endpoint) {
        packet<BUFFER_SIZE> pkt = build_packet(0, 0, 0, UNRELIABLE, endpoint, message);
        queuePacketForSending(pkt);
    }

    // retry functions
    void queuePacketForRetry(const packet<BUFFER_SIZE>& pkt) {
        {
            std::lock_guard<std::mutex> lock(_retryQueueMutex);
            _retryQueue.push_back(pkt);
        }
    }

    std::unordered_set<asio::ip::udp::endpoint, EndpointHash, EndpointEqual> getKnownClients() {
        return _knownClients;
    }
    // overload for vector<char>
    const std::array<char, BUFFER_SIZE> getLastReliablePacketData() {
        std::lock_guard<std::mutex> lock(_unprocessedReliableMessagesDataMutex);
        if (_unprocessedReliableMessagesData.empty()) return {};

        std::array<char, BUFFER_SIZE> message =
            std::move(_unprocessedReliableMessagesData.back().first);
        _unprocessedReliableMessagesData.erase(_unprocessedReliableMessagesData.end() - 1);

        return message;
    }
    const std::array<char, BUFFER_SIZE> getLastUnreliablePacketData() {
        std::lock_guard<std::mutex> lock(_unprocessedUnreliableMessagesDataMutex);
        if (_unprocessedUnreliableMessagesData.empty()) return {};
        std::array<char, BUFFER_SIZE> message =
            std::move(_unprocessedUnreliableMessagesData.back().first);
        _unprocessedUnreliableMessagesData.erase(_unprocessedUnreliableMessagesData.end() - 1);
        return message;
    }

    std::vector<std::array<char, BUFFER_SIZE>> getUnreliableMessagesFromEndpointData(
        const asio::ip::udp::endpoint& endpoint) {
        std::vector<std::array<char, BUFFER_SIZE>> messages;
        std::lock_guard<std::mutex>                lock(_unprocessedUnreliableMessagesDataMutex);
        for (auto it = _unprocessedUnreliableMessagesData.begin();
             it != _unprocessedUnreliableMessagesData.end();) {
            if (it->second == endpoint) {
                messages.push_back(std::move(it->first));
                it = _unprocessedUnreliableMessagesData.erase(it);
            } else {
                ++it;
            }
        }
        return messages;
    }
    std::vector<std::array<char, BUFFER_SIZE>> getReliableMessagesFromEndpointData(
        const asio::ip::udp::endpoint& endpoint) {
        std::vector<std::array<char, BUFFER_SIZE>> messages;
        std::lock_guard<std::mutex>                lock(_unprocessedReliableMessagesDataMutex);
        for (auto it = _unprocessedReliableMessagesData.begin();
             it != _unprocessedReliableMessagesData.end();) {
            if (it->second == endpoint) {
                messages.push_back(std::move(it->first));
                it = _unprocessedReliableMessagesData.erase(it);
            } else {
                ++it;
            }
        }
        return messages;
    }

    // void               retransmit_unacknowledged_packets(const asio::ip::udp::endpoint&
    // endpoint);
    void print_packet(const packet<BUFFER_SIZE>& pkt) {
        std::cout << "Packet: " << std::endl;
        std::cout << "Sequence number: " << pkt.sequenceNb << std::endl;
        std::cout << "Start sequence number: " << pkt.startSequenceNb << std::endl;
        std::cout << "End sequence number: " << pkt.endSequenceNb << std::endl;
        std::cout << "Packet size: " << pkt.packetSize << std::endl;
        std::cout << "Flag: " << pkt.flag << std::endl;
        std::cout << "Data: " << std::string(pkt.data.begin(), pkt.data.end()) << std::endl;
    }

    std::function<void(const asio::ip::udp::endpoint& client_endpoint)> _newClientCallback;

   private:
    asio::steady_timer     _retransmissionTimer;
    asio::ip::udp::socket& _socket;
    Role                   _role;

    std::unordered_set<asio::ip::udp::endpoint, EndpointHash, EndpointEqual> _knownClients;

    bool                  _stopProcessing;
    std::array<char, 800> _recvBuffer;

    asio::ip::udp::endpoint _endpoint;  // the endpoint that sent the last message

    // threads
    std::thread _sendPacketThread;
    std::thread _receivePacketThread;
    std::thread _retryPacketThread;

    // receive packets variables
    SEQUENCE_TYPE _messageId;  // start at 0 and increment of (packet number) for each message
    std::mutex    _message_id_mutex;

    std::unordered_map<int, std::vector<packet<BUFFER_SIZE>>> _receivedPackets;
    std::mutex                                                _receivedPacketsMutex;

    // send packets variables
    std::deque<packet<BUFFER_SIZE>>         _sendQueue;
    std::mutex                              _sendQueueMutex;
    std::condition_variable                 _sendQueueCv;
    std::unordered_set<packet<BUFFER_SIZE>> _sendQueueSet;

    // retry packets variables
    std::deque<packet<BUFFER_SIZE>> _retryQueue;
    std::mutex                      _retryQueueMutex;

    std::vector<std::pair<std::array<char, BUFFER_SIZE>, asio::ip::udp::endpoint>>
               _unprocessedUnreliableMessagesData;
    std::mutex _unprocessedUnreliableMessagesDataMutex;

    std::vector<std::pair<std::array<char, BUFFER_SIZE>, asio::ip::udp::endpoint>>
               _unprocessedReliableMessagesData;
    std::mutex _unprocessedReliableMessagesDataMutex;

    // work guard
    asio::executor_work_guard<asio::io_context::executor_type> _work_guard;
};

#endif  // SENDPACKETS_HPP