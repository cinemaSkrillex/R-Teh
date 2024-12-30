/*
** EPITECH PROJECT, 2024
** r-teh-mega-backup
** File description:
** RtypeServerProtocol
*/

#ifndef RTYPESERVERPROTOCOL_HPP_
#define RTYPESERVERPROTOCOL_HPP_

#include <vector>

namespace RTypeProtocol {

enum MessageType : int { NEW_CLIENT = 0x01, PLAYER_MOVE = 0x02, EVENT_MESSAGE = 0x03 };

struct BaseMessage {
    int message_type;
    int uuid;
};

struct PlayerMoveMessage : BaseMessage {
    int  x;
    int  y;
    long timestamp;

    struct EventMessage : BaseMessage {
        std::vector<std::pair<int, std::vector<char>>> components;  // Component-based payload
    };

    // Unified serialize/deserialize
    std::vector<char> serialize(const BaseMessage& msg);
    BaseMessage       deserialize(const std::vector<char>& buffer);

    // Helper functions for specific message types
    std::vector<char> serialize(const PlayerMoveMessage& msg);
    std::vector<char> serialize(const EventMessage& msg);
    PlayerMoveMessage deserializePlayerMove(const std::vector<char>& buffer);
    EventMessage      deserializeEventMessage(const std::vector<char>& buffer);
};

}  // namespace RTypeProtocol
#endif /* !RTYPESERVERPROTOCOL_HPP_ */
