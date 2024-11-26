/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** UDPServer.cpp
*/

#include "../../include/Server/UDPServer.hpp"
#include <bits/this_thread_sleep.h>
#include <system_error>

UDPServer::UDPServer(asio::io_context& io_context, unsigned short port)
    : socket_(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)),
      retransmission_timer_(io_context), packet_manager_(io_context, socket_, Role::SERVER) {
    start_receive();
}

UDPServer::~UDPServer() { std::cout << "deleting UDPServer"; }

void UDPServer::start_receive() {
    socket_.async_receive_from(asio::buffer(recv_buffer_), remote_endpoint_,
                               [this](std::error_code ec, std::size_t bytes_recvd) {
                                   if (!ec && bytes_recvd > 0) {
                                       std::cout << "bytes_recvd: " << bytes_recvd << std::endl;
                                       handle_receive(bytes_recvd);
                                   } else {
                                       start_receive();
                                   }
                               });
}

void UDPServer::handle_ack(const std::string& ack_message) {
    packet_manager_.handle_ack(ack_message);
}

void UDPServer::handle_receive(std::size_t bytes_recvd) {
    std::vector<char> buffer(recv_buffer_.data(), recv_buffer_.data() + bytes_recvd);
    packet            pkt = deserialize_packet(buffer);
    std::cout << "Received packet with sequence number: " << pkt.sequence_no << std::endl;
    switch (pkt.flag) {
    case ACK:
        handle_ack(std::string(pkt.data.begin(), pkt.data.end()));
        break;
    case RELIABLE:
        handle_reliable_packet(std::string(pkt.data.begin(), pkt.data.end()), pkt.sequence_no);
        break;
    case UNRELIABLE:
        handle_unreliable_packet(std::string(pkt.data.begin(), pkt.data.end()));
        break;
    default:
        std::cerr << "Received unknown packet type: " << pkt.flag << std::endl;
        break;
    }
    handle_new_client(remote_endpoint_);
    start_receive();
}

void UDPServer::handle_reliable_packet(const std::string& message, int sequence_number) {
    std::cout << "received UDPServer::handle_reliable_packet" << sequence_number
              << "remote: " << remote_endpoint_ << std::endl;
    // Process the message content
    packet_manager_.send_ack(sequence_number, remote_endpoint_);
}

void UDPServer::handle_unreliable_packet(const std::string& message) {
    std::cout << "Processing unreliable message: " << message << std::endl;
}

void UDPServer::handle_new_client(const asio::ip::udp::endpoint& client_endpoint) {
    if (known_clients_.find(client_endpoint) == known_clients_.end()) {
        known_clients_.insert(client_endpoint);
        std::cout << "New client connected: " << client_endpoint << std::endl;

        std::string boat_info =
            "Boats are watercraft of various sizes designed to float, plane, work, or travel on "
            "water. "
            "They are typically smaller than ships, which are generally distinguished by their "
            "larger size, "
            "shape, cargo or passenger capacity, or their ability to carry boats. "
            "Boats have been used since prehistoric times and have been essential for fishing, "
            "transportation, "
            "trade, and warfare. Modern boats are usually powered by engines, but many still use "
            "sails or oars. "
            "There are many types of boats, including sailboats, motorboats, fishing boats, and "
            "rowboats. ";

        // Repeat the boat_info string to exceed 10,000 bytes
        std::string long_boat_info;
        while (long_boat_info.size() <= 100000) {
            long_boat_info += boat_info;
        }
        // Send test packets to the new client
        // send_unreliable_packet("Unreliable packet from server", client_endpoint);
        // send_reliable_packet("Reliable packet from server", client_endpoint);
        send_reliable_packet(long_boat_info, client_endpoint);
    }
}

/**
 * @brief Sends an unreliable packet to the specified endpoint.
 *
 * This function sends a packet containing the given message to the specified
 * endpoint using UDP. The packet is sent asynchronously and is considered
 * unreliable, meaning it is not critical if the packet is lost during
 * transmission (e.g., player position updates).
 *
 * @param message The message to be sent in the packet.
 * @param endpoint The destination endpoint to which the packet will be sent.
 */
void UDPServer::send_unreliable_packet(const std::string&             message,
                                       const asio::ip::udp::endpoint& endpoint) {
    packet_manager_.send_unreliable_packet(message, endpoint);
}
/**
 * @brief Sends a reliable packet over UDP.
 *
 * This function sends a packet to the specified endpoint and ensures that the packet is reliably
 * delivered by implementing an acknowledgment mechanism. If the packet is not acknowledged within
 * a certain time frame, it will be resent.
 *
 * @param message The message to be sent.
 * @param endpoint The destination endpoint to which the message should be sent.
 */
void UDPServer::send_reliable_packet(const std::string&             message,
                                     const asio::ip::udp::endpoint& endpoint) {
    packet_manager_.send_reliable_packet(message, endpoint);
}

const std::unordered_set<asio::ip::udp::endpoint, EndpointHash, EndpointEqual>&
UDPServer::get_known_clients() const {
    return known_clients_;
}