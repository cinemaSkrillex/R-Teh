
#include "../include/ECS/Systems/AISystem.hpp"

#include <iostream>
namespace RealEngine {

AISystem::AISystem() {}

AISystem::~AISystem() {}

void AISystem::update(Registry& registry, float deltaTime) {
    auto entities = registry.view<AI>();

    if (entities.empty()) {
        return;
    }
    for (auto entity : entities) {
        auto* ai     = registry.getComponent<AI>(entity);
        auto* target = registry.getComponent<Target>(entity);

        if (ai->active) {
            if (target) {
                if (ai->behaviorOnTarget) {
                    ai->behaviorOnTarget(registry, entity, deltaTime);
                }
            } else {
                if (ai->behaviorPassive) {
                    ai->behaviorPassive(registry, entity, deltaTime);
                }
            }
        }
    }
}

void AISystem::attachPassiveBehavior(Registry& registry, Entity entity,
                                     std::function<void(Registry&, Entity, float)> behavior) {
    auto* ai = registry.getComponent<AI>(entity);

    if (ai) {
        ai->behaviorPassive = behavior;
    } else {
        std::cerr << "Error: Entity does not have an AI component!" << std::endl;
    }
}
void AISystem::attachTargetBehavior(Registry& registry, Entity entity,
                                    std::function<void(Registry&, Entity, float)> behavior) {
    auto* ai     = registry.getComponent<AI>(entity);
    auto* target = registry.getComponent<Target>(entity);

    if (ai) {
        ai->behaviorOnTarget = behavior;
    } else {
        std::cerr << "Error: Entity does not have an AI component!" << std::endl;
    }
}

void AISystem::setActive(Registry& registry, Entity entity, bool active) {
    auto* ai = registry.getComponent<AI>(entity);
    if (ai) {
        ai->active = active;
    } else {
        std::cerr << "Error: Entity does not have an AI component!" << std::endl;
    }
}
}  // namespace RealEngine
