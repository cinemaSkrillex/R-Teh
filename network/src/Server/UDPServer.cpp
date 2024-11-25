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

void UDPServer::send_packet(const packet& pkt, const asio::ip::udp::endpoint& endpoint) {
    const auto buffer = std::make_shared<std::vector<char>>(serialize_packet(pkt));

    socket_.async_send_to(asio::buffer(*buffer), endpoint,
                          [this, pkt, endpoint](std::error_code ec, std::size_t /*bytes_sent*/) {
                              if (!ec) {
                                  std::cout
                                      << "Sent packet with sequence number: " << pkt.sequence_no
                                      << std::endl;
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

        // Send test packets to the new client
        // send_unreliable_packet("Unreliable packet from server", client_endpoint);
        // send_reliable_packet("Reliable packet from server", client_endpoint);
        send_reliable_packet(
            "Reliable packet "
            "vgzefgvzebhezbjhgnrgknergnjergnkergjernjgerkjgknergknergknerkgnerkngernkgerkngernkgjer"
            "ngnkrengrekjvgzefgvzebhezbjhgnrgknergnjergnkergjernjgerkjgknergknergknerkgnerkngernkge"
            "rkngernkgjerngnkrengrekjvgzefgvzebhezbjhgnrgknergnjergnkergjernjgerkjgknergknergknerkg"
            "nerkngernkgerkngernkgjerngnkrengrekjvgzefgvzebhezbjhgnrgknergnjergnkergjernjgerkjgkner"
            "gknergknerkgnerkngernkgerkngernkgjerngnkrengrekjvgzefgvzebhezbjhgnrgknergnjergnkergjer"
            "njgerkjgknergknergknerkgnerkngernkgerkngernkgjerngnkrengrekjvgzefgvzebhezbjhgnrgknergn"
            "jergnkergjernjgerkjgknergknergknerkgnerkngernkgerkngernkgjerngnkrengrekjvgzefgvzebhezb"
            "jhgnrgknergnjergnkergjernjgerkjgknergknergknerkgnerkngernkgerkngernkgjerngnkrengrekjvg"
            "zefgvzebhezbjhgnrgknergnjergnkergjernjgerkjgknergknergknerkgnerkngernkgerkngernkgjerng"
            "nkrengrekjvgzefgvzebhezbjhgnrgknergnjergnkergjernjgerkjgknergknergknerkgnerkngernkgerk"
            "ngernkgjerngnkrengrekjvgzefgvzebhezbjhgnrgknergnjergnkergjernjgerkjgknergknergknerkgne"
            "rkngernkgerkngernkgjerngnkrengrekjvgzefgvzebhezbjhgnrgknergnjergnkergjernjgerkjgknergk"
            "nergknerkgnerkngernkgerkngernkgjerngnkrengrekjvgzefgvzebhezbjhgnrgknergnjergnkergjernj"
            "gerkjgknergknergknerkgnerkngernkgerkngernkgjerngnkrengrekjvgzefgvzebhezbjhgnrgknergnje"
            "rgnkergjernjgerkjgknergknergknerkgnerkngernkgerkngernkgjerngnkrengrekjvgzefgvzebhezbjh"
            "gnrgknergnjergnkergjernjgerkjgknergknergknerkgnerkngernkgerkngernkgjerngnkrengrekjvgze"
            "fgvzebhezbjhgnrgknergnjergnkergjernjgerkjgknergknergknerkgnerkngernkgerkngernkgjerngnk"
            "rengrekjvgzefgvzebhezbjhgnrgknergnjergnkergjernjgerkjgknergknergknerkgnerkngernkgerkng"
            "ernkgjerngnkrengrekjvgzefgvzebhezbjhgnrgknergnjergnkergjernjgerkjgknergknergknerkgnerk"
            "ngernkgerkngernkgjerngnkrengrekjvgzefgvzebhezbjhgnrgknergnjergnkergjernjgerkjgknergkne"
            "rgknerkgnerkngernkgerkngernkgjerngnkrengrekjvgzefgvzebhezbjhgnrgknergnjergnkergjernjge"
            "rkjgknergknergknerkgnerkngernkgerkngernkgjerngnkrengrekjvgzefgvzebhezbjhgnrgknergnjerg"
            "nkergjernjgerkjgknergknergknerkgnerkngernkgerkngernkgjerngnkrengrekjvgzefgvzebhezbjhgn"
            "rgknergnjergnkergjernjgerkjgknergknergknerkgnerkngernkgerkngernkgjerngnkrengrekjvgzefg"
            "vzebhezbjhgnrgknergnjergnkergjernjgerkjgknergknergknerkgnerkngernkgerkngernkgjerngnkre"
            "ngrekjvgzefgvzebhezbjhgnrgknergnjergnkergjernjgerkjgknergknergknerkgnerkngernkgerknger"
            "nkgjerngnkrengrekjvgzefgvzebhezbjhgnrgknergnjergnkergjernjgerkjgknergknergknerkgnerkng"
            "ernkgerkngernkgjerngnkrengrekjvgzefgvzebhezbjhgnrgknergnjergnkergjernjgerkjgknergknerg"
            "knerkgnerkngernkgerkngernkgjerngnkrengrekjvgzefgvzebhezbjhgnrgknergnjergnkergjernjgerk"
            "jgknergknergknerkgnerkngernkgerkngernkgjerngnkrengrekjvgzefgvzebhezbjhgnrgknergnjergnk"
            "ergjernjgerkjgknergknergknerkgnerkngernkgerkngernkgjerngnkrengrekjvgzefgvzebhezbjhgnrg"
            "knergnjergnkergjernjgerkjgknergknergknerkgnerkngernkgerkngernkgjerngnkrengrekjvgzefgvz"
            "ebhezbjhgnrgknergnjergnkergjernjgerkjgknergknergknerkgnerkngernkgerkngernkgjerngnkreng"
            "rekjvgzefgvzebhezbjhgnrgknergnjergnkergjernjgerkjgknergknergknerkgnerkngernkgerkngernk"
            "gjerngnkrengrekjvgzefgvzebhezbjhgnrgknergnjergnkergjernjgerkjgknergknergknerkgnerknger"
            "nkgerkngernkgjerngnkrengrekjvgzefgvzebhezbjhgnrgknergnjergnkergjernjgerkjgknergknergkn"
            "erkgnerkngernkgerkngernkgjerngnkrengrekjvgzefgvzebhezbjhgnrgknergnjergnkergjernjgerkjg"
            "knergknergknerkgnerkngernkgerkngernkgjerngnkrengrekjvgzefgvzebhezbjhgnrgknergnjergnker"
            "gjernjgerkjgknergknergknerkgnerkngernkgerkngernkgjerngnkrengrekjvgzefgvzebhezbjhgnrgkn"
            "ergnjergnkergjernjgerkjgknergknergknerkgnerkngernkgerkngernkgjerngnkrengrekjvgzefgvzeb"
            "hezbjhgnrgknergnjergnkergjernjgerkjgknergknergknerkgnerkngernkgerkngernkgjerngnkrengre"
            "kjvgzefgvzebhezbjhgnrgknergnjergnkergjernjgerkjgknergknergknerkgnerkngernkgerkngernkgj"
            "erngnkrengrekjvgzefgvzebhezbjhgnrgknergnjergnkergjernjgerkjgknergknergknerkgnerkngernk"
            "gerkngernkgjerngnkrengrekjvgzefgvzebhezbjhgnrgknergnjergnkergjernjgerkjgknergknergkner"
            "kgnerkngernkgerkngernkgjerngnkrengrekjvgzefgvzebhezbjhgnrgknergnjergnkergjernjgerkjgkn"
            "ergknergknerkgnerkngernkgerkngernkgjerngnkrengrekjvgzefgvzebhezbjhgnrgknergnjergnkergj"
            "ernjgerkjgknergknergknerkgnerkngernkgerkngernkgjerngnkrengrekj",
            client_endpoint);
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

extern "C" SERVER_API UDPServer* create_udp_server(asio::io_context& io_context,
                                                   unsigned short    port) {
    return new UDPServer(io_context, port);
}