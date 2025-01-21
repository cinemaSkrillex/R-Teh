/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** EyeBomber
*/

#include "Game/Mobs/EyeBomber.hpp"

namespace rtype {

static void simpleBehavior(RealEngine::Registry& registry, RealEngine::Entity entity,
                           float deltaTime) {
    registry.addComponent(entity, RealEngine::Acceleration{-20.0f, 0.0f, 0.5f});
    goStraightConstant(registry, entity, deltaTime);
}

static void adjustSpeedAndRush(RealEngine::Registry& registry, RealEngine::Entity entity,
                               float deltaTime) {
    registry.addComponent(entity, RealEngine::Acceleration{60.0f, 30.0f, 0.5f});
    rushAndAimTowardsTargetZigzagging(registry, entity, deltaTime);
}

EyeBomber::EyeBomber(RealEngine::Registry& registry, sf::Vector2f position)
    : _entity(registry.spawnEntity()) {
    registry.addComponent(_entity, RealEngine::Position{position.x, position.y});
    registry.addComponent(_entity,
                          RealEngine::Interpolation{
                              {position.x, position.y}, {position.x, position.y}, 0.f, 1.f, false});
    auto spriteSheet = *RealEngine::AssetManager::getInstance().getSpriteSheet("eye_bomber");
    registry.addComponent(_entity, RealEngine::SpriteSheet{spriteSheet});
    registry.addComponent(_entity, RealEngine::Velocity{0.0f, 0.0f, {135.0f, 135.0f}, 0.8f});
    registry.addComponent(_entity, RealEngine::Acceleration{60.0f, 30.0f, 0.5f});
    registry.addComponent(_entity, RealEngine::Rotation{180.0f});
    registry.addComponent(_entity,
                          RealEngine::Collision{{0.0f, 0.0f, 15.f * GAME_SCALE, 10.f * GAME_SCALE},
                                                "mob",
                                                false,
                                                RealEngine::CollisionType::ENEMY,
                                                takesDamage});

    registry.addComponent(_entity, RealEngine::Health{15, 15});
    registry.addComponent(_entity, RealEngine::TargetRadius{300.0f});
    registry.addComponent(_entity, RealEngine::Score{30});
    registry.addComponent(_entity, RealEngine::AI{adjustSpeedAndRush, simpleBehavior, true});
    registry.addComponent(_entity, RealEngine::Damage{15});
    registry.addComponent(_entity, RealEngine::AutoDestructible{-1.0f, true, false});
    registry.addComponent(
        _entity,
        RealEngine::NetvarContainer{
            {{"sprite_name", {"string", "sprite_name", std::string("eye_bomber"), nullptr}},
             {"new_entity", {"bool", "new_entity", true, nullptr}},
             {"entity_type", {"int", "entity_type", 2, nullptr}},
             {"destroy_out_of_screen", {"bool", "destroy_out_of_screen", false, nullptr}},
             {"powerup_drop", {"float", "powerup_drop", 10.f, nullptr}},
             {"powerup_type", {"int", "powerup_type", 1, nullptr}}}});
}

EyeBomber::~EyeBomber() {}

void EyeBomber::setTarget(std::shared_ptr<RealEngine::Entity> target,
                          RealEngine::Registry&               registry) {
    auto* acceleration = registry.getComponent<RealEngine::Acceleration>(_entity);

    registry.addComponent(_entity, RealEngine::Target{target});
}

}  // namespace rtype