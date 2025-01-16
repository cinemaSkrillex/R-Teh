//
// Created by binosspc on 1/12/25.
//

#ifndef AEVENT_HPP
#define AEVENT_HPP

#include <array>

#include "IEvent.hpp"

class AEvent : public IEvent {
   public:
    void broadcastAllReliable(const std::array<char, 800>& message, RtypeServer* server);
    void broadcastAllUnreliable(const std::array<char, 800>& message, RtypeServer* server);
};
#endif  // AEVENT_HPP
