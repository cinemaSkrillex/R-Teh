#pragma once

#include "Engine.hpp"
#include "Macros.hpp"

namespace rtype {

enum EyeBossState { SHORT_RANGE, MID_RANGE, LONG_RANGE };

class EyeBoss {
   public:
    EyeBoss(RealEngine::Registry& registry);
    ~EyeBoss();
    void targetBossBehavior(RealEngine::Registry& registry, RealEngine::Entity target,
                            float deltaTime);
    void setTarget(std::shared_ptr<RealEngine::Entity> target);
    void aimAtTarget(RealEngine::Position* targetPosition, float rotationSpeed, float deltaTime);
    void noTargetBossBehavior(RealEngine::Registry& registry, float deltaTime);
    void shortRangeBehavior(RealEngine::Registry& registry, RealEngine::Entity target);
    void midRangeBehavior(RealEngine::Registry& registry, RealEngine::Entity target);
    void longRangeBehavior(RealEngine::Registry& registry, RealEngine::Entity target);
    void circularAttack(RealEngine::Registry& registry, RealEngine::Entity target, float deltaTime);
    void shootLaser();
    void setBossStatus(int state);
    std::shared_ptr<RealEngine::Entity> getEntity() { return _entity; }

   private:
    RealEngine::Registry&                               _registry;
    std::shared_ptr<RealEngine::Entity>                 _entity;
    RealEngine::Sprite                                  _shortSprite;
    RealEngine::Sprite                                  _midSprite;
    RealEngine::Sprite                                  _longSprite;
    RealEngine::Sprite                                  _laserSprite;
    std::unordered_map<std::string, RealEngine::Sprite> _bossSheet;
    EyeBossState                                        _state;
    float                                               _shootCooldown;
    float                                               _shootPhaseTimer;
    bool                                                _isInShootPhase;
    std::vector<std::shared_ptr<RealEngine::Entity>>    _laserEntities;
    std::vector<std::shared_ptr<RealEngine::Entity>>    _laserEntitiesToDelete;
};
}  // namespace rtype