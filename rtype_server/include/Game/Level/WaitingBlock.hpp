/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** WaitingBlock
*/

#ifndef WAITINGBLOCK_HPP_
#define WAITINGBLOCK_HPP_
#include "BaseBlock.hpp"

namespace rtype {

class WaitingBlock : public BaseBlock {
   public:
    WaitingBlock(RealEngine::Registry& registry, sf::Vector2f position,
                 const std::string& spriteName, float rotation,
                 RealEngine::CollisionType collisionType);
    ~WaitingBlock();

    void initialize(RealEngine::Registry& registry, sf::Vector2f position,
                    const std::string& spriteName, float rotation, float scrollingSpeed,
                    RealEngine::CollisionType collisionType) override;

    void waitingBlockCollisionHandler(RealEngine::CollisionType collisionType,
                                      RealEngine::Registry& registry, RealEngine::Entity collider,
                                      RealEngine::Entity entity);
    int  getPlayersInBox() { return _playersInBox; }
    void ReinitPlayersInBox() { _playersInBox = 0; }
    std::shared_ptr<RealEngine::Entity> getEntity() override { return _waitingEntity; }
    const std::string&                  getElement() const override { return _element; }

   private:
    std::shared_ptr<RealEngine::Entity> _waitingEntity;
    RealEngine::Sprite                  _waitingBlockSprite;
    std::string                         _element;
    int                                 _playersInBox;
};

}  // namespace rtype
#endif /* !WAITINGBLOCK_HPP_ */
