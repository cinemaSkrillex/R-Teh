/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** PacketManager.cpp
*/

#include "../../include/shared/PacketManager.hpp"

PacketManager::PacketManager(asio::io_context& io_context, asio::ip::udp::socket& socket, Role role)
    : _message_id(0),
      retransmission_timer_(io_context),
      socket_(socket),
      role_(role),
      _stop_processing(false),
      work_guard_(asio::make_work_guard(io_context)) {}

PacketManager::~PacketManager() {
    _stop_processing = true;
    if (_send_packet_thread.joinable()) _send_packet_thread.join();
    if (_receive_packet_thread.joinable()) _receive_packet_thread.join();
    if (_retry_packet_thread.joinable()) _retry_packet_thread.join();
}

void PacketManager::start() {
    _receive_packet_thread = std::thread(&PacketManager::receive, this);
    _send_packet_thread    = std::thread(&PacketManager::send, this);
    _retry_packet_thread   = std::thread(&PacketManager::retry, this);
}

packet PacketManager::build_packet(int sequence_nb, int start_sequence_nb, int end_sequence_nb,
                                   Flags flag, const asio::ip::udp::endpoint& endpoint,
                                   const std::array<char, BUFFER_SIZE>& message) {
    packet pkt;
    pkt.sequence_nb       = sequence_nb;
    pkt.start_sequence_nb = start_sequence_nb;
    pkt.end_sequence_nb   = end_sequence_nb;
    pkt.packet_size       = std::min(BUFFER_SIZE, static_cast<int>(message.size()));

    size_t start_idx = sequence_nb * BUFFER_SIZE;
    size_t end_idx   = start_idx + pkt.packet_size;

    if (start_idx > message.size()) {
        std::cerr << "Error: Start index out of range!" << std::endl;
        std::cerr << "Start index: " << start_idx << " Message size: " << message.size()
                  << std::endl;
        return {};  // Return an empty packet or handle the error
    }
    pkt.data.assign(message.begin() + start_idx,
                    message.begin() + std::min(end_idx, message.size()));

    pkt.flag     = flag;
    pkt.endpoint = endpoint;
    return pkt;
}

// threads functions

void PacketManager::receive() {
    socket_.async_receive_from(
        asio::buffer(recv_buffer_), _endpoint, [this](std::error_code ec, std::size_t bytes_recvd) {
            if (!ec && bytes_recvd > 0) {
                handle_receive(bytes_recvd);
            } else {
                if (ec) {
                    std::cerr << "Receive error: " << ec.message() << std::endl;
                } else {
                    std::cerr << "Received 0 bytes" << std::endl;
                }
            }
            receive();
        });
}

void PacketManager::send() {
    while (!_stop_processing) {
        std::unique_lock<std::mutex> lock(_send_queue_mutex);
        _send_queue_cv.wait(lock, [this] { return !_send_queue.empty() || _stop_processing; });
        while (!_send_queue.empty()) {
            auto pkt = _send_queue.front();
            _send_queue.pop_front();
            _send_queue_set.erase(pkt);
            lock.unlock();
            send_packet(pkt);
            lock.lock();
        }
    }
}

void PacketManager::retry() {
    while (!_stop_processing) {
        // 150 ms is the time to wait before retrying to send a packet - Windows may need to disable
        // firewall
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
        {
            std::lock_guard<std::mutex> lock(_retry_queue_mutex);
            for (const auto& pkt : _retry_queue) {
                send_packet(pkt);
            }
        }
    }
}

// receive functions

void PacketManager::handle_receive(std::size_t bytes_recvd) {
    // auto message = std::make_shared<std::string>(recv_buffer_.data(), bytes_recvd);
    // packet pkt     = deserialize_packet(std::vector<char>(message->begin(), message->end()));
    auto   message = std::make_shared<std::vector<char>>(recv_buffer_.begin(),
                                                         recv_buffer_.begin() + bytes_recvd);
    packet pkt     = deserialize_packet(*message);
    switch (pkt.flag) {
        case ACK:
            handle_ack(std::string(pkt.data.begin(), pkt.data.end()));
            break;
        case RELIABLE:
            handle_reliable_packet(pkt);
            break;
        case UNRELIABLE:
            // handle_unreliable_packet(std::string(pkt.data.begin(), pkt.data.end()));
            handle_unreliable_packet(pkt.data);
            break;
        case NEW_CLIENT:
            handle_new_client(_endpoint);
            break;
        case TEST:
            handle_test(_endpoint);
            break;
        default:
            std::cerr << "Received unknown packet type: " << pkt.flag << std::endl;
            break;
    }
}

void PacketManager::handle_ack(const std::string& ack_message) {
    SEQUENCE_TYPE sequence_start  = 0;
    SEQUENCE_TYPE sequence_number = 0;

    if (ack_message.size() < 4) {
        std::cerr << "Invalid ACK message size" << std::endl;
        return;
    }
    if (ack_message.substr(0, 11) == "CLIENT_ACK:") {
        sequence_start  = std::stoi(ack_message.substr(11, ack_message.find(',')));
        sequence_number = std::stoi(ack_message.substr(ack_message.find(',') + 1));
    } else if (ack_message.substr(0, 4) == "ACK:") {
        sequence_start  = std::stoi(ack_message.substr(4, ack_message.find(',')));
        sequence_number = std::stoi(ack_message.substr(ack_message.find(',') + 1));
    } else {
        std::cerr << "Invalid ACK message: " << ack_message << std::endl;
        return;
    }
    {
        std::lock_guard<std::mutex> lock(_retry_queue_mutex);
        for (auto it = _retry_queue.begin(); it != _retry_queue.end();) {
            if (it->sequence_nb == sequence_number && it->start_sequence_nb == sequence_start) {
                _retry_queue.erase(it);
                break;
            }
            ++it;
        }
    }
}

void PacketManager::handle_reliable_packet(const packet& pkt) {
    bool all_packets_received = false;

    if (pkt.sequence_nb > pkt.end_sequence_nb) {
        std::cerr << "Invalid sequence number: " << pkt.sequence_nb << std::endl;
        return;
    }

    {
        std::lock_guard<std::mutex> lock(_received_packets_mutex);
        // check that the packet is not already in the map
        std::vector<packet> temp_vector = _received_packets[pkt.start_sequence_nb];
        if (std::find(temp_vector.begin(), temp_vector.end(), pkt) != temp_vector.end()) {
        } else {
            _received_packets[pkt.start_sequence_nb].push_back(pkt);
        }

        int final_size    = pkt.end_sequence_nb - pkt.start_sequence_nb + 1;
        int received_size = _received_packets[pkt.start_sequence_nb].size();

        if (received_size == final_size) {
            all_packets_received = true;
        }
    }

    if (all_packets_received) {
        std::vector<char> complete_data;
        {
            std::lock_guard<std::mutex> lock(_received_packets_mutex);
            // WARNING: sort could make the program slower
            std::sort(
                _received_packets[pkt.start_sequence_nb].begin(),
                _received_packets[pkt.start_sequence_nb].end(),
                [](const packet& a, const packet& b) { return a.sequence_nb < b.sequence_nb; });

            for (const auto& packet : _received_packets[pkt.start_sequence_nb]) {
                complete_data.insert(complete_data.end(), packet.data.begin(), packet.data.end());
            }
            _received_packets.erase(pkt.start_sequence_nb);
            const std::string message = std::string(complete_data.begin(), complete_data.end());

            // Process the message content
            std::lock_guard<std::mutex> unprocessed_lock(_unprocessed_reliable_messages_mutex);
            // For now: we will push with the last _endpoint. Can cause issues in the future
            _unprocessed_reliable_messages.push_back(std::make_pair(message, _endpoint));
        }
    }

    send_ack(pkt.start_sequence_nb, pkt.sequence_nb, _endpoint);
}

// did not change for now
void PacketManager::handle_unreliable_packet(const std::string& message) {
    // Process the message content
    std::lock_guard<std::mutex> lock(_unprocessed_unreliable_messages_mutex);
    _unprocessed_unreliable_messages.push_back(std::make_pair(message, _endpoint));
}

void PacketManager::handle_unreliable_packet(const std::vector<char>& message) {
    // Process the message content
    std::lock_guard<std::mutex> lock(_unprocessed_unreliable_messages_data_mutex);
    _unprocessed_unreliable_messages_data.push_back(std::make_pair(message, _endpoint));
}

void PacketManager::handle_new_client(const asio::ip::udp::endpoint& client_endpoint) {
    if (_known_clients.find(client_endpoint) != _known_clients.end()) return;

    _known_clients.insert(client_endpoint);
    // here we used to send test message. TODO test flag to trigger the test

    if (_new_client_callback) _new_client_callback(client_endpoint);
}

static std::string testPacketManager() {
    std::string boat_info =
        "Boats are watercraft of various sizes designed to float, plane, work, or travel "
        "on "
        "water. "
        "They are typically smaller than ships, which are generally distinguished by their "
        "larger size, "
        "shape, cargo or passenger capacity, or their ability to carry boats. "
        "Boats have been used since prehistoric times and have been essential for fishing, "
        "transportation, "
        "trade, and warfare. Modern boats are usually powered by engines, but many still "
        "use "
        "sails or oars. "
        "There are many types of boats, including sailboats, motorboats, fishing boats, "
        "and "
        "rowboats. ";

    // Repeat the boat_info string to exceed x bytes
    // 1000 byes = 1KB
    // 1000000 bytes = 1MB
    // 10000000 bytes = 10MB
    // 100000000 bytes = 100MB
    // 1000000000 bytes = 1GB No need to go further in UDP.
    std::string long_boat_info;
    while (long_boat_info.size() <= 1000000) {
        long_boat_info += boat_info;
    }
    return long_boat_info;
}

void PacketManager::handle_test(const asio::ip::udp::endpoint& endpoint) {
    // send reliable packet to the client with message testPacketManager()
    // send_reliable_packet(testPacketManager(), endpoint);
}

// send functions

void PacketManager::send_ack(SEQUENCE_TYPE start_sequence_number, SEQUENCE_TYPE sequence_number,
                             const asio::ip::udp::endpoint& endpoint_) {
    std::string ack_message;
    if (role_ == Role::SERVER) {
        ack_message = "CLIENT_ACK:" + std::to_string(start_sequence_number) + "," +
                      std::to_string(sequence_number);
    }
    if (role_ == Role::CLIENT) {
        ack_message =
            "ACK:" + std::to_string(start_sequence_number) + "," + std::to_string(sequence_number);
    }

    // packet pkt = build_packet(0, 0, 0, ACK, endpoint_, ack_message);
    // queue_packet_for_sending(pkt);
}

void PacketManager::queue_packet_for_sending(const packet& pkt) {
    {
        std::lock_guard<std::mutex> lock(_send_queue_mutex);
        if (_send_queue_set.find(pkt) == _send_queue_set.end()) {
            _send_queue.emplace_back(pkt);
            _send_queue_set.insert(pkt);
        } else {
        }
    }
    _send_queue_cv.notify_one();
}

void PacketManager::send_packet(const packet& pkt) {
    const auto buffer = std::make_shared<std::vector<char>>(serialize_packet(pkt));
    std::this_thread::sleep_for(std::chrono::nanoseconds(100000));
    socket_.async_send_to(asio::buffer(*buffer), pkt.endpoint,
                          [this, buffer](std::error_code ec, std::size_t bytes_sent) {
                              if (ec) {
                                  std::cerr << "Send error: " << ec.message()
                                            << " size: " << bytes_sent << std::endl;
                              }
                          });
}

void PacketManager::send_reliable_packet(const std::array<char, BUFFER_SIZE>& message,
                                         const asio::ip::udp::endpoint&       endpoint) {
    int total_packets = (message.size() + BUFFER_SIZE - 1) / BUFFER_SIZE;

    int start_sequence_nb;
    int end_sequence_nb;

    {
        std::lock_guard<std::mutex> lock(_message_id_mutex);
        start_sequence_nb = _message_id;
        end_sequence_nb   = _message_id + total_packets - 1;
    }

    for (int i = 0; i < total_packets; ++i) {
        packet pkt;

        {
            std::lock_guard<std::mutex> lock(_message_id_mutex);
            pkt = build_packet(_message_id - start_sequence_nb, start_sequence_nb, end_sequence_nb,
                               RELIABLE, endpoint, message);
            _message_id++;
        }

        if (endpoint.address().is_unspecified()) {
            std::cerr << "Server endpoint is unspecified" << std::endl;
            return;
        }
        if (pkt.sequence_nb > pkt.end_sequence_nb) {
            std::cerr << "sending reliable Invalid sequence number: " << pkt.sequence_nb
                      << " start_sequence_nb: " << pkt.start_sequence_nb
                      << " end_sequence_nb: " << pkt.end_sequence_nb << std::endl;
            continue;
        }
        queue_packet_for_sending(pkt);
        queue_packet_for_retry(pkt);
    }
}

// void PacketManager::send_unreliable_packet(const std::string&             message,
//                                            const asio::ip::udp::endpoint& endpoint) {
//     packet pkt = build_packet(0, 0, 0, UNRELIABLE, endpoint, message);
//     queue_packet_for_sending(pkt);
// }

void PacketManager::send_unreliable_packet(const std::array<char, BUFFER_SIZE>& message,
                                           const asio::ip::udp::endpoint&       endpoint) {
    // print the vector of char
    std::cout << "Sending message: ";
    for (auto i : message) {
        std::cout << "{" << i << "}";
    }
    packet pkt = build_packet(0, 0, 0, UNRELIABLE, endpoint, message);
    std::cout << "Packet data: ";
    for (auto i : pkt.data) {
        std::cout << std::hex << static_cast<unsigned int>(i) << " ";
    }
    std::cout << std::endl;
    queue_packet_for_sending(pkt);
}

void PacketManager::send_new_client(const asio::ip::udp::endpoint& endpoint) {
    // packet pkt = build_packet(0, 0, 0, NEW_CLIENT, endpoint, "");
    // queue_packet_for_sending(pkt);
}

void PacketManager::send_test(const asio::ip::udp::endpoint& endpoint) {
    // packet pkt = build_packet(0, 0, 0, TEST, endpoint, "");
    // queue_packet_for_sending(pkt);
}

// retry functions

void PacketManager::queue_packet_for_retry(const packet& pkt) {
    {
        std::lock_guard<std::mutex> lock(_retry_queue_mutex);
        _retry_queue.push_back(pkt);
    }
}

std::unordered_set<asio::ip::udp::endpoint, EndpointHash, EndpointEqual>
PacketManager::getKnownClients() {
    return _known_clients;
}

const std::array<char, BUFFER_SIZE> PacketManager::get_last_reliable_packet_data() {
    std::lock_guard<std::mutex> lock(_unprocessed_reliable_messages_mutex);
    if (_unprocessed_reliable_messages.empty()) return {};
    std::array<char, BUFFER_SIZE> message;
    std::copy(_unprocessed_reliable_messages.back().first.begin(),
              _unprocessed_reliable_messages.back().first.end(), message.begin());
    _unprocessed_reliable_messages.erase(_unprocessed_reliable_messages.end() - 1);
    return message;
}

const std::array<char, BUFFER_SIZE> PacketManager::get_last_unreliable_packet_data() {
    std::lock_guard<std::mutex> lock(_unprocessed_unreliable_messages_data_mutex);
    if (_unprocessed_unreliable_messages_data.empty()) return {};
    std::array<char, BUFFER_SIZE> message;
    std::copy(_unprocessed_unreliable_messages_data.back().first.begin(),
              _unprocessed_unreliable_messages_data.back().first.end(), message.begin());
    _unprocessed_unreliable_messages_data.erase(_unprocessed_unreliable_messages_data.end() - 1);
    return message;
}

std::vector<std::array<char, BUFFER_SIZE>>
PacketManager::get_unreliable_messages_from_endpoint_data(const asio::ip::udp::endpoint& endpoint) {
    std::vector<std::array<char, BUFFER_SIZE>> messages;
    std::lock_guard<std::mutex>                lock(_unprocessed_unreliable_messages_data_mutex);
    for (auto it = _unprocessed_unreliable_messages_data.begin();
         it != _unprocessed_unreliable_messages_data.end();) {
        if (it->second == endpoint) {
            std::array<char, BUFFER_SIZE> message_array;
            std::copy(it->first.begin(), it->first.end(), message_array.begin());
            messages.push_back(message_array);
            it = _unprocessed_unreliable_messages_data.erase(it);
        } else {
            ++it;
        }
    }
    return messages;
}

std::vector<std::array<char, BUFFER_SIZE>> PacketManager::get_reliable_messages_from_endpoint_data(
    const asio::ip::udp::endpoint& endpoint) {
    std::vector<std::array<char, BUFFER_SIZE>> messages;
    std::lock_guard<std::mutex>                lock(_unprocessed_reliable_messages_mutex);
    for (auto it = _unprocessed_reliable_messages.begin();
         it != _unprocessed_reliable_messages.end();) {
        if (it->second == endpoint) {
            std::array<char, BUFFER_SIZE> message_array;
            std::copy(it->first.begin(), it->first.end(), message_array.begin());
            messages.push_back(message_array);
            it = _unprocessed_reliable_messages.erase(it);
        } else {
            ++it;
        }
    }
    return messages;
}

void PacketManager::print_packet(const packet& pkt) {
    std::cout << "Packet: " << std::endl;
    std::cout << "Sequence number: " << pkt.sequence_nb << std::endl;
    std::cout << "Start sequence number: " << pkt.start_sequence_nb << std::endl;
    std::cout << "End sequence number: " << pkt.end_sequence_nb << std::endl;
    std::cout << "Packet size: " << pkt.packet_size << std::endl;
    std::cout << "Flag: " << pkt.flag << std::endl;
    std::cout << "Data: " << std::string(pkt.data.begin(), pkt.data.end()) << std::endl;
}