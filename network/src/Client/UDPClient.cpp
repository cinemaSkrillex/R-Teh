/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** UDPClient.cpp
*/

#include "../../include/Client/UDPClient.hpp"
#include <thread>

UDPClient::UDPClient(asio::io_context& io_context, unsigned short port)
    : socket_(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)), server_endpoint_(),
      recv_buffer_(), sequence_number_(0), retransmission_timer_(io_context),
      packet_manager_(io_context, socket_, Role::CLIENT) {
    start_receive();
    processing_thread_ = std::thread(&UDPClient::process_packets, this);
}

void UDPClient::start_receive() {
    socket_.async_receive_from(asio::buffer(recv_buffer_), server_endpoint_,
                               [this](std::error_code ec, std::size_t bytes_recvd) {
                                   if (!ec && bytes_recvd > 0) {
                                       handle_receive(bytes_recvd);
                                   }
                                   start_receive();
                               });
}

void UDPClient::handle_ack(const std::string& ack_message) {
    // CLIENT_ACK:
    // std::uint32_t sequence_number = std::stoul(ack_message.substr(11));
    // if (sequence_number < 0) {
    //     std::cerr << "Invalid sequence number: " << sequence_number << std::endl;
    //     return;
    // }
    // std::cout << "Received ACK for sequence number: " << sequence_number << std::endl;
    // unacknowledged_packets_.erase(sequence_number);
    packet_manager_.handle_ack(ack_message);
}

// void UDPClient::handle_receive(std::size_t bytes_recvd) {
//     std::vector<char> buffer(recv_buffer_.data(), recv_buffer_.data() + bytes_recvd);
//     packet            pkt = deserialize_packet(buffer);
//     switch (pkt.flag) {
//     case ACK:
//         handle_ack(std::string(pkt.data.begin(), pkt.data.end()));
//         break;
//     case RELIABLE:
//         handle_reliable_packet(pkt);
//         break;
//     case UNRELIABLE:
//         handle_unreliable_packet(std::string(pkt.data.begin(), pkt.data.end()));
//         break;
//     default:
//         std::cerr << "Received unknown packet type: " << pkt.flag << std::endl;
//         break;
//     }
// }

void UDPClient::handle_receive(std::size_t bytes_recvd) {
    auto   message = std::make_shared<std::string>(recv_buffer_.data(), bytes_recvd);
    packet pkt     = deserialize_packet(std::vector<char>(message->begin(), message->end()));

    {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        packet_queue_.push(pkt);
    }
    queue_cv_.notify_one();

    start_receive();
}

void UDPClient::process_packets() {
    while (true) {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        queue_cv_.wait(lock, [this] { return !packet_queue_.empty() || stop_processing_; });

        if (stop_processing_ && packet_queue_.empty()) {
            break;
        }

        packet pkt = packet_queue_.front();
        packet_queue_.pop();
        lock.unlock();

        switch (pkt.flag) {
        case RELIABLE:
            handle_reliable_packet(pkt);
            break;
        default:
            std::cerr << "Received unknown packet type: " << pkt.flag << std::endl;
            break;
        }
    }
}

// void UDPClient::handle_reliable_packet(const packet& pkt) {
//     // Print the received packet details
//     std::cout << "Received packet with sequence number: " << pkt.sequence_no << std::endl;
//     std::cout << "Packet size: " << pkt.packet_size << std::endl;
//     std::cout << "Data: " << std::string(pkt.data.begin(), pkt.data.end()) << std::endl;

//     // Send ACK back to the server
//     packet_manager_.send_ack(pkt.sequence_no, server_endpoint_);
// }

void UDPClient::handle_reliable_packet(const packet& pkt) {
    // Print the received packet details
    std::cout << "Received packet with sequence number: " << pkt.sequence_no << std::endl;
    std::cout << "Packet size: " << pkt.packet_size << std::endl;
    std::cout << "Data: " << std::string(pkt.data.begin(), pkt.data.end()) << std::endl;

    // Store the packet
    received_packets_[pkt.sequence_no] = pkt;

    // Set start and max sequence numbers
    if (start_sequence_no_ == -1) {
        start_sequence_no_ = pkt.start_sequence_no;
        end_sequence_no    = pkt.end_sequence_no;
    }

    // Check if all packets have been received
    if (received_packets_.size() == (end_sequence_no - start_sequence_no_ + 1)) {
        // All packets received, reassemble the message
        std::vector<char> complete_data;
        for (int i = start_sequence_no_; i <= end_sequence_no; ++i) {
            const auto& pkt = received_packets_[i];
            complete_data.insert(complete_data.end(), pkt.data.begin(), pkt.data.end());
        }

        std::string complete_message(complete_data.begin(), complete_data.end());
        std::cout << "Complete message: " << complete_message << std::endl;
        std::cout << "Total size received: " << complete_data.size() << " bytes" << std::endl;

        // Clear the received packets map for the next message
        received_packets_.clear();
        start_sequence_no_ = -1;
        end_sequence_no    = -1;
    }

    // Send ACK back to the server
    packet_manager_.send_ack(pkt.sequence_no, server_endpoint_);
}

void UDPClient::send_ack(std::uint32_t sequence_number) {
    std::string ack_message = "ACK:" + std::to_string(sequence_number);
    packet      pkt;
    pkt.sequence_no = sequence_number;
    pkt.packet_size = ack_message.size();
    pkt.flag        = ACK;
    pkt.data.assign(ack_message.begin(), ack_message.end());

    // Serialize the packet using the serialize_packet method
    const auto buffer = std::make_shared<std::vector<char>>(serialize_packet(pkt));
    std::cout << "Sending ACK for sequence number: " << sequence_number << " to "
              << server_endpoint_ << std::endl;

    if (server_endpoint_.address().is_unspecified()) {
        std::cerr << "Server endpoint is unspecified" << std::endl;
        return;
    }

    socket_.async_send_to(asio::buffer(*buffer), server_endpoint_,
                          [this, buffer](std::error_code ec, std::size_t bytes_sent) {
                              if (ec) {
                                  std::cerr << "Send ACK error: " << ec.message()
                                            << " size: " << bytes_sent << std::endl;
                              }
                          });
}

void UDPClient::send_unreliable_packet(const std::string&             message,
                                       const asio::ip::udp::endpoint& server_endpoint) {
    packet_manager_.send_unreliable_packet(message, server_endpoint);
}

void UDPClient::send_reliable_packet(const std::string&             message,
                                     const asio::ip::udp::endpoint& endpoint) {
    packet_manager_.send_reliable_packet(message, endpoint);
}

void UDPClient::handle_unreliable_packet(const std::string& message) {
    // Process the message content
    std::cout << "Processing unreliable message: " << message << std::endl;
}

extern "C" CLIENT_API UDPClient* create_udp_client(asio::io_context& io_context,
                                                   unsigned short    port) {
    return new UDPClient(io_context, port);
}