#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>

#include "ECS/Components/Drawable.hpp"
#include "ECS/Components/Position.hpp"
#include "ECS/Components/Rotation.hpp"
#include "ECS/Components/Sprite.hpp"
#include "ECS/Components/SpriteSheet.hpp"
#include "ECS/Entities/Entity.hpp"
#include "ECS/Registry/Registry.hpp"
#include "Media/Graphics/Rendering/Sprite.hpp"

namespace rtype {

enum EyeBossState { SHORT_RANGE, MID_RANGE, LONG_RANGE };

class EyeBoss {
   public:
    EyeBoss(RealEngine::Registry& registry);
    ~EyeBoss();
    void               shortRangeBehavior();
    void               midRangeBehavior();
    void               longRangeBehavior();
    void               setBossStatus(int state);
    RealEngine::Entity getEntity() { return _entity; }

   private:
    RealEngine::Registry&                               _registry;
    RealEngine::Entity                                  _entity;
    RealEngine::Sprite                                  _shortSprite;
    RealEngine::Sprite                                  _midSprite;
    RealEngine::Sprite                                  _longSprite;
    std::unordered_map<std::string, RealEngine::Sprite> _bossSheet;
    EyeBossState                                        _state;
};
}  // namespace rtype