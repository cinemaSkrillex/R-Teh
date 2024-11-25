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
    // std::uint32_t sequence_number = std::stoul(ack_message.substr(4));
    // std::cout << "Received ACK for sequence number: " << sequence_number << std::endl;
    // unacknowledged_packets_.erase(sequence_number);
}

// void UDPServer::handle_receive(std::size_t bytes_recvd) {
//     auto message = std::make_shared<std::string>(recv_buffer_.data(), bytes_recvd);
//     std::cout << "Received: " << *message << "bytes_recvd" << bytes_recvd << std::endl;
//     if (message->substr(0, 4) == "ACK:") {
//         handle_ack(*message);
//     } else {
//         handle_new_client(remote_endpoint_);
//     }
//     start_receive();
// }

void UDPServer::handle_receive(std::size_t bytes_recvd) {
    std::vector<char> buffer(recv_buffer_.data(), recv_buffer_.data() + bytes_recvd);
    packet            pkt = deserialize_packet(buffer);
    std::cout << "Received packet with sequence number: " << pkt.sequence_no << std::endl;
    switch (pkt.flag) {
    case ACK:
        handle_ack(std::string(pkt.data.begin(), pkt.data.end()));
        break;
    case RELIABLE:
        handle_reliable_packet(std::string(pkt.data.begin(), pkt.data.end()), pkt.packet_size);
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

void UDPServer::handle_reliable_packet(const std::string& message, std::size_t colon_pos) {
    std::uint32_t sequence_number = std::stoul(message.substr(0, colon_pos));
    std::cout << "received UDPServer::handle_reliable_packet" << sequence_number
              << "remote: " << remote_endpoint_ << std::endl;
    // Process the message content
    std::cout << "Processing reliable message: " << message.substr(colon_pos + 1) << std::endl;
    start_receive();
}

void UDPServer::handle_unreliable_packet(const std::string& message) {
    std::cout << "Processing unreliable message: " << message << std::endl;
}

void UDPServer::send_client_ack(std::uint32_t sequence_number) {
    std::string ack_message = "CLIENT_ACK:" + std::to_string(sequence_number);
    std::cout << "Sending ACK: " << ack_message << " to " << remote_endpoint_ << std::endl;
    auto buffer = std::make_shared<std::string>(ack_message);
    if (remote_endpoint_.address().is_unspecified()) {
        std::cerr << "Remote endpoint is unspecified" << std::endl;
        return;
    }
    socket_.async_send_to(asio::buffer(*buffer), remote_endpoint_,
                          [this, buffer](std::error_code ec, std::size_t bytes_sent) {
                              if (ec) {
                                  std::cerr << "Send ACK error: " << ec.message()
                                            << " size: " << bytes_sent << std::endl;
                              }
                          });
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
    // int total_packets = (message.size() + BUFFER_SIZE - 1) / BUFFER_SIZE;
    // int packets_sent  = 0;
    // std::cout << "Sending " << total_packets << " packets" << "size: " << message.size()
    //           << std::endl;

    // for (int i = 0; i < total_packets; ++i) {
    //     packet pkt;
    //     pkt.sequence_no = sequence_number_++;
    //     pkt.packet_size = std::min(BUFFER_SIZE, static_cast<int>(message.size() - i *
    //     BUFFER_SIZE)); pkt.flag        = RELIABLE; pkt.data.assign(message.begin() + i *
    //     BUFFER_SIZE,
    //                     message.begin() + i * BUFFER_SIZE + pkt.packet_size);
    //     unacknowledged_packets_[pkt.sequence_no] = pkt;
    //     send_packet(pkt, endpoint);
    // }
    // schedule_retransmissions(endpoint);
}

// void UDPServer::retransmit_unacknowledged_packets(const asio::ip::udp::endpoint& endpoint) {
//     for (const auto& pair : unacknowledged_packets_) {
//         std::cout << "Retransmitting packet with sequence number: " << pair.first << std::endl;
//         send_packet(pair.second, endpoint);
//     }
// }

// void UDPServer::schedule_retransmissions(const asio::ip::udp::endpoint& endpoint) {
//     retransmission_timer_.expires_after(std::chrono::milliseconds(300));
//     retransmission_timer_.async_wait([this, endpoint](const std::error_code& ec) {
//         if (!ec) {
//             retransmit_unacknowledged_packets(endpoint);
//             for (const auto& pair : unacknowledged_packets_) {
//                 std::cout << "Unacknowledged packet: " << pair.first << std::endl;
//             }
//             if (!unacknowledged_packets_.empty()) {
//                 schedule_retransmissions(endpoint);
//             }
//         }
//     });
// }

const std::unordered_set<asio::ip::udp::endpoint, EndpointHash, EndpointEqual>&
UDPServer::get_known_clients() const {
    return known_clients_;
}

extern "C" SERVER_API UDPServer* create_udp_server(asio::io_context& io_context,
                                                   unsigned short    port) {
    return new UDPServer(io_context, port);
}