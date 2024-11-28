
#include "../include/ECS/Systems/AISystem.hpp"

#include <iostream>
namespace RealEngine {

AISystem::AISystem() {}

AISystem::~AISystem() {}

void AISystem::update(Registry& registry, float deltaTime) {
    // std::cout << "AISystem::update" << std::endl;
    return;
}

void AISystem::attachBehavior(Registry& registry, float deltaTime) { return; }
}  // namespace RealEngine
