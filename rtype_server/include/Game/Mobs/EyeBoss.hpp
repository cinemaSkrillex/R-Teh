/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** EyeBoss
*/

#pragma once

#include "Game/Mobs/Behaviors.hpp"
#include "Game/Mobs/Projectiles/EyeLaser.hpp"

namespace rtype {

enum EyeBossState { SHORT_RANGE, MID_RANGE, LONG_RANGE };

class EyeBoss {
   public:
    EyeBoss(RealEngine::Registry& registry);
    ~EyeBoss();
    void setTarget(RealEngine::Registry& registry, std::shared_ptr<RealEngine::Entity> target);
    std::shared_ptr<RealEngine::Entity> getEntity() { return _entity; }

   private:
    std::shared_ptr<RealEngine::Entity>                 _entity;
    std::unordered_map<std::string, RealEngine::Sprite> _bossSheet;
    EyeBossState                                        _state;
    float                                               _shootCooldown;
    float                                               _shootPhaseTimer;
    bool                                                _isInShootPhase;
    std::vector<std::shared_ptr<RealEngine::Entity>>    _laserEntities;
    std::vector<std::shared_ptr<RealEngine::Entity>>    _laserEntitiesToDelete;
};
}  // namespace rtype