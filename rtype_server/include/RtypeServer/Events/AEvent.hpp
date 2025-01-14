//
// Created by binosspc on 1/12/25.
//

#ifndef AEVENT_HPP
#define AEVENT_HPP

#include <SFML/System/Vector2.hpp>
#include <array>
#include <string>

#include "../ECS/Components/Velocity.hpp"
#include "IEvent.hpp"

class AEvent : public IEvent {
   public:
    void broadcastAllReliable(const std::array<char, 800>& message, RtypeServer* server);
    void broadcastAllUnreliable(const std::array<char, 800>& message, RtypeServer* server);
    std::array<char, 800> createBulletMessage(long int id, sf::Vector2f position,
                                              std::string          sprite_name,
                                              RealEngine::Velocity velocity = {
                                                  500, 0, {500, 500}, 0});
};
#endif  // AEVENT_HPP
