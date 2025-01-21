/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** BaseBlock
*/

#ifndef BASEBLOCK_HPP_
#define BASEBLOCK_HPP_

#include <SFML/System/Vector2.hpp>
#include <memory>
#include <string>

#include "../../Engine.hpp"

namespace rtype {

class BaseBlock {
   public:
    virtual ~BaseBlock()                                             = default;
    virtual void initialize(RealEngine::Registry& registry, sf::Vector2f position,
                            const std::string& spriteName, float rotation, float scrollingSpeed,
                            RealEngine::CollisionType collisionType) = 0;
    virtual std::shared_ptr<RealEngine::Entity> getEntity()          = 0;
    virtual const std::string&                  getElement() const   = 0;
};

}  // namespace rtype

#endif /* !BASEBLOCK_HPP_ */
