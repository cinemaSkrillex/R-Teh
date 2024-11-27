/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** PacketManager.cpp
*/

#include "../../include/shared/PacketManager.hpp"

PacketManager::PacketManager(asio::io_context& io_context, asio::ip::udp::socket& socket, Role role)
    : sequence_number_(0), retransmission_timer_(io_context), socket_(socket), role_(role),
      stop_processing_(false), work_guard_(asio::make_work_guard(io_context)) {}

PacketManager::~PacketManager() {
    stop_processing_ = true;
    queue_cv_.notify_all();
    if (receive_packet_thread_.joinable())
        receive_packet_thread_.join();
    if (process_packet_thread_.joinable())
        process_packet_thread_.join();
    if (send_packet_thread_.joinable())
        send_packet_thread_.join();
    if (retransmission_thread_.joinable())
        retransmission_thread_.join();
}

void PacketManager::start() {
    receive_packet_thread_ = std::thread(&PacketManager::start_receive, this);
    process_packet_thread_ = std::thread(&PacketManager::process_packets, this);
    send_packet_thread_    = std::thread(&PacketManager::send_packets, this);
    retransmission_thread_ = std::thread(&PacketManager::handle_retransmissions, this);
}

void PacketManager::start_receive() {
    socket_.async_receive_from(asio::buffer(recv_buffer_), remote_endpoint_,
                               [this](std::error_code ec, std::size_t bytes_recvd) {
                                   if (!ec && bytes_recvd > 0) {
                                       handle_receive(bytes_recvd);
                                   } else {
                                       if (ec) {
                                           std::cerr << "Receive error: " << ec.message()
                                                     << std::endl;
                                       } else {
                                           std::cerr << "Received 0 bytes" << std::endl;
                                       }
                                   }
                                   start_receive();
                               });
}

void PacketManager::handle_receive(std::size_t bytes_recvd) {
    auto   message = std::make_shared<std::string>(recv_buffer_.data(), bytes_recvd);
    packet pkt     = deserialize_packet(std::vector<char>(message->begin(), message->end()));

    {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        packet_queue_.push(pkt);
    }
    queue_cv_.notify_one();
}

void PacketManager::process_packets() {
    while (!stop_processing_) {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        // std::cout << "packet_queue_.size(): " << packet_queue_.size() << std::endl;
        queue_cv_.wait(lock, [this] { return !packet_queue_.empty() || stop_processing_; });

        while (!packet_queue_.empty()) {
            packet pkt = packet_queue_.front();
            packet_queue_.pop();
            switch (pkt.flag) {
            case ACK:
                handle_ack(std::string(pkt.data.begin(), pkt.data.end()));
                break;
            case RELIABLE:
                handle_reliable_packet(pkt);
                break;
            case UNRELIABLE:
                handle_unreliable_packet(std::string(pkt.data.begin(), pkt.data.end()));
                break;
            default:
                std::cerr << "Received unknown packet type: " << pkt.flag << std::endl;
                break;
            }
            if (Role::SERVER == role_) {
                handle_new_client(remote_endpoint_);
            }
        }
    }
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
    while (long_boat_info.size() <= 10000000) {
        long_boat_info += boat_info;
    }
    return long_boat_info;
}

void PacketManager::handle_new_client(const asio::ip::udp::endpoint& client_endpoint) {
    if (known_clients_.find(client_endpoint) != known_clients_.end())
        return;

    known_clients_.insert(client_endpoint);
    std::cout << "New client connected: " << client_endpoint << std::endl;
    // send_reliable_packet(testPacketManager(), client_endpoint);
}

void PacketManager::handle_reliable_packet(const packet& pkt) {
    if (pkt.sequence_no < 0 || pkt.sequence_no > pkt.end_sequence_no ||
        pkt.sequence_no < pkt.start_sequence_no) {
        std::cerr << "Invalid sequence number: " << pkt.sequence_no << std::endl;
        return;
    }

    bool all_packets_received = false;
    {
        std::lock_guard<std::mutex> lock(received_packets_mutex_);
        received_packets_[pkt.sequence_no] = pkt;

        if (start_sequence_no_ == -1) {
            start_sequence_no_ = pkt.start_sequence_no;
            end_sequence_no    = pkt.end_sequence_no;
        }

        // Check if all packets have been received
        int final_size    = end_sequence_no - start_sequence_no_ + 1;
        int received_size = received_packets_.size();
        std::cout << "Received packets: " << received_size << " out of " << final_size << " packets"
                  << std::endl;

        if (received_size == final_size) {
            all_packets_received = true;
        }
    }

    if (all_packets_received) {
        std::cout << "Reassembling message" << std::endl;
        // All packets received, reassemble the message
        std::vector<char> complete_data;
        {
            std::lock_guard<std::mutex> lock(received_packets_mutex_);
            for (int i = start_sequence_no_; i <= end_sequence_no; ++i) {
                const auto& pkt = received_packets_[i];
                complete_data.insert(complete_data.end(), pkt.data.begin(), pkt.data.end());
            }
            std::cout << "Complete message: "
                      << std::string(complete_data.begin(), complete_data.end()) << std::endl;
            std::cout << "Total size received: " << complete_data.size() << " bytes" << std::endl;
        }

        // Clear the received packets map for the next message
        {
            std::lock_guard<std::mutex> lock(received_packets_mutex_);
            received_packets_.clear();
            start_sequence_no_ = -1;
            end_sequence_no    = -1;
        }
    }

    // Send ACK back to the server
    send_ack(pkt.sequence_no, remote_endpoint_);
}

void PacketManager::handle_unreliable_packet(const std::string& message) {
    // Process the message content
    std::cout << "Processing unreliable message: " << message << std::endl;
    std::lock_guard<std::mutex> lock(_unprocessed_unreliable_messages_mutex);
    _unprocessed_unreliable_messages.push(message);
}

void PacketManager::send_packets() {
    while (!stop_processing_) {
        std::unique_lock<std::mutex> lock(send_queue_mutex_);
        std::cout << "send_queue_.size(): " << send_queue_.size() << std::endl;
        send_queue_cv_.wait(lock, [this] { return !send_queue_.empty() || stop_processing_; });
        while (!send_queue_.empty()) {
            auto [pkt, endpoint] = send_queue_.front();
            send_queue_.pop();
            send_queue_set_.erase(pkt.sequence_no);
            if (pkt.sequence_no < 0 || (pkt.sequence_no > pkt.end_sequence_no ||
                                        pkt.sequence_no < pkt.start_sequence_no) &&
                                           pkt.flag == RELIABLE) {
                if (pkt.sequence_no < 0) {
                    std::cerr << "send Invalid sequence number: " << pkt.sequence_no << std::endl;
                } else {
                    std::cerr << "send Invalid sequence number: " << pkt.sequence_no
                              << " start_sequence_no: " << pkt.start_sequence_no
                              << " end_sequence_no: " << pkt.end_sequence_no << std::endl;
                }
                lock.unlock();
                continue;
            }
            lock.unlock(); // Unlock the mutex while sending the packet
            send_packet(pkt, endpoint);

            // std::this_thread::sleep_for(std::chrono::milliseconds(30));
            lock.lock(); // Lock the mutex again before checking the queue
        }
    }
}

void PacketManager::handle_retransmissions() {
    while (!stop_processing_) {
        std::unique_lock<std::mutex> lock(retransmission_queue_mutex_);
        std::cout << "retransmission_queue_.size(): " << retransmission_queue_.size() << std::endl;
        retransmission_queue_cv_.wait(
            lock, [this] { return !retransmission_queue_.empty() || stop_processing_; });

        while (!retransmission_queue_.empty()) {
            auto [pkt, endpoint] = retransmission_queue_.front();
            retransmission_queue_.pop_front();
            retransmission_queue_set_.erase(pkt.sequence_no);
            if (pkt.sequence_no < 0 || pkt.sequence_no > pkt.end_sequence_no ||
                pkt.sequence_no < pkt.start_sequence_no) {
                if (pkt.sequence_no < 0) {
                    std::cerr << "retransmission Invalid sequence number: " << pkt.sequence_no
                              << std::endl;
                } else {
                    std::cerr << "retransmission Invalid sequence number: " << pkt.sequence_no
                              << " start_sequence_no: " << pkt.start_sequence_no
                              << " end_sequence_no: " << pkt.end_sequence_no << std::endl;
                }
                lock.unlock();
                continue;
            }

            lock.unlock(); // Unlock the mutex while sending the packet

            send_packet(pkt, endpoint);

            std::cout << "retransmissionqueue.size(): " << retransmission_queue_.size()
                      << std::endl;
            if (!retransmission_queue_.empty()) {
                std::cout << "retransmissionqueue.front().first.sequence_no: "
                          << retransmission_queue_.front().first.sequence_no << std::endl;
            }
            lock.lock(); // Lock the mutex again before checking the queue
        }
    }
}

void PacketManager::queue_packet_for_retransmission(const packet&                  pkt,
                                                    const asio::ip::udp::endpoint& endpoint) {
    {
        std::lock_guard<std::mutex> lock(retransmission_queue_mutex_);
        if (retransmission_queue_set_.find(pkt.sequence_no) == retransmission_queue_set_.end()) {
            retransmission_queue_.emplace_back(pkt, endpoint);
            retransmission_queue_set_.insert(pkt.sequence_no);
            std::cout << "Queued packet for retransmission: " << pkt.sequence_no << std::endl;
        } else {
            std::cout << "Packet already in retransmission queue: " << pkt.sequence_no << std::endl;
        }
    }
    retransmission_queue_cv_.notify_one();
}

void PacketManager::schedule_retransmissions(const asio::ip::udp::endpoint& endpoint) {
    retransmission_timer_.expires_after(std::chrono::milliseconds(300));
    retransmission_timer_.async_wait([this, endpoint](const std::error_code& ec) {
        if (!ec) {
            std::lock_guard<std::mutex> lock(retransmission_queue_mutex_);
            for (const auto& pair : retransmission_queue_) {
                if (received_packets_.find(pair.first.sequence_no) == received_packets_.end()) {
                    std::cout << "Retransmitting unacknowledged packet: " << pair.first.sequence_no
                              << std::endl;
                    queue_packet_for_retransmission(pair.first, endpoint);
                }
            }
            if (!retransmission_queue_.empty()) {
                schedule_retransmissions(endpoint);
            }
        }
    });
}

void PacketManager::handle_ack(const std::string& ack_message) {
    std::uint32_t sequence_number = 0;
    // std::cout << "ack_message: " << ack_message << std::endl;
    if (ack_message.size() < 4) {
        std::cerr << "Invalid ACK message: " << ack_message << std::endl;
        return;
    }
    if (ack_message.substr(0, 11) == "CLIENT_ACK:") {
        sequence_number = std::stoul(ack_message.substr(11));
    } else if (ack_message.substr(0, 4) == "ACK:") {
        sequence_number = std::stoul(ack_message.substr(4));
    } else {
        std::cerr << "Invalid ACK message: " << ack_message << std::endl;
        return;
    }
    {
        std::lock_guard<std::mutex> lock(retransmission_queue_mutex_);
        retransmission_queue_set_.erase(sequence_number);
        // Remove the packet from the retransmission queue
        auto it = std::remove_if(retransmission_queue_.begin(), retransmission_queue_.end(),
                                 [sequence_number](const auto& pair) {
                                     return pair.first.sequence_no == sequence_number;
                                 });
        retransmission_queue_.erase(it, retransmission_queue_.end());
        // std::cout << "ACK received for packet: " << sequence_number << std::endl;
    }
}

void PacketManager::send_reliable_packet(const std::string&             message,
                                         const asio::ip::udp::endpoint& endpoint) {
    int total_packets = (message.size() + BUFFER_SIZE - 1) / BUFFER_SIZE;
    std::cout << "Sending " << total_packets << " packets" << " size: " << message.size()
              << std::endl;

    int start_sequence_no;
    int end_sequence_no;

    {
        std::lock_guard<std::mutex> lock(sequence_number_mutex_);
        start_sequence_no = sequence_number_;
        end_sequence_no   = sequence_number_ + total_packets - 1;
    }

    for (int i = 0; i < total_packets; ++i) {
        packet pkt;
        {
            std::lock_guard<std::mutex> lock(sequence_number_mutex_);
            pkt.sequence_no = sequence_number_++;
        }
        pkt.start_sequence_no = start_sequence_no;
        pkt.end_sequence_no   = end_sequence_no;
        pkt.packet_size = std::min(BUFFER_SIZE, static_cast<int>(message.size() - i * BUFFER_SIZE));
        pkt.flag        = RELIABLE;
        pkt.data.assign(message.begin() + i * BUFFER_SIZE,
                        message.begin() + i * BUFFER_SIZE + pkt.packet_size);
        if (endpoint.address().is_unspecified()) {
            std::cerr << "Server endpoint is unspecified" << std::endl;
            return;
        }
        if (pkt.sequence_no < 0 || pkt.sequence_no > pkt.end_sequence_no ||
            pkt.sequence_no < pkt.start_sequence_no) {
            if (pkt.sequence_no < 0) {
                std::cerr << "sending reliable Invalid sequence number: " << pkt.sequence_no
                          << std::endl;
            } else {
                std::cerr << "sending reliable Invalid sequence number: " << pkt.sequence_no
                          << " start_sequence_no: " << pkt.start_sequence_no
                          << " end_sequence_no: " << pkt.end_sequence_no << std::endl;
            }
            continue;
        }
        queue_packet_for_sending(pkt, endpoint);
        queue_packet_for_retransmission(pkt, endpoint);
    }
}

void PacketManager::queue_packet_for_sending(const packet&                  pkt,
                                             const asio::ip::udp::endpoint& endpoint) {
    {
        std::lock_guard<std::mutex> lock(send_queue_mutex_);
        // Check if the current packet is not already in the send queue
        if (send_queue_set_.find(pkt.sequence_no) == send_queue_set_.end()) {
            send_queue_.emplace(pkt, endpoint);
            send_queue_set_.insert(pkt.sequence_no);
            std::cout << "Queued packet for sending: " << pkt.sequence_no << std::endl;
        } else {
            std::cout << "Packet already in send queue: " << pkt.sequence_no << std::endl;
        }
    }
    send_queue_cv_.notify_one();
}

void PacketManager::send_unreliable_packet(const std::string&             message,
                                           const asio::ip::udp::endpoint& endpoint) {
    packet pkt;
    pkt.sequence_no = 0; // no validation for unreliable packets
    pkt.packet_size = message.size();
    pkt.flag        = UNRELIABLE;
    pkt.data.assign(message.begin(), message.end());

    // Serialize the packet using the serialize_packet method
    queue_packet_for_sending(pkt, endpoint);
}

void PacketManager::send_ack(std::uint32_t                  sequence_number,
                             const asio::ip::udp::endpoint& endpoint_) {
    std::string ack_message;
    if (role_ == Role::SERVER) {
        ack_message = "CLIENT_ACK:" + std::to_string(sequence_number);
    }
    if (role_ == Role::CLIENT) {
        ack_message = "ACK:" + std::to_string(sequence_number);
    }
    packet pkt;
    pkt.sequence_no = sequence_number;
    pkt.packet_size = ack_message.size();
    pkt.flag        = ACK;
    pkt.data.assign(ack_message.begin(), ack_message.end());

    // std::cout << "Sending ACK: " << ack_message << std::endl;
    if (endpoint_.address().is_unspecified()) {
        std::cerr << "Server endpoint is unspecified" << std::endl;
        return;
    }
    queue_packet_for_sending(pkt, endpoint_);
}

std::queue<packet> PacketManager::get_received_packets() {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    return packet_queue_;
}

std::string PacketManager::get_last_unreliable_packet() {
    std::lock_guard<std::mutex> lock(_unprocessed_unreliable_messages_mutex);
    if (_unprocessed_unreliable_messages.empty())
        return "";
    std::string                 message = _unprocessed_unreliable_messages.top();
    _unprocessed_unreliable_messages.pop();
    return message;
}

void PacketManager::send_packet(const packet& pkt, const asio::ip::udp::endpoint& endpoint) {
    // Serialize the packet using the serialize_packet method
    const auto buffer = std::make_shared<std::vector<char>>(serialize_packet(pkt));
    // print_packet(pkt);
    // sleep for 30ms when sleeping it works better.
    std::this_thread::sleep_for(std::chrono::nanoseconds(1000000));
    socket_.async_send_to(asio::buffer(*buffer), endpoint,
                          [this, buffer](std::error_code ec, std::size_t bytes_sent) {
                              if (ec) {
                                  std::cerr << "Send error: " << ec.message()
                                            << " size: " << bytes_sent << std::endl;
                              }
                          });
}

void PacketManager::print_packet(const packet& pkt) {
    std::cout << "Packet: " << std::endl;
    std::cout << "Sequence number: " << pkt.sequence_no << std::endl;
    std::cout << "Start sequence number: " << pkt.start_sequence_no << std::endl;
    std::cout << "End sequence number: " << pkt.end_sequence_no << std::endl;
    std::cout << "Packet size: " << pkt.packet_size << std::endl;
    std::cout << "Flag: " << pkt.flag << std::endl;
    std::cout << "Data: " << std::string(pkt.data.begin(), pkt.data.end()) << std::endl;
}