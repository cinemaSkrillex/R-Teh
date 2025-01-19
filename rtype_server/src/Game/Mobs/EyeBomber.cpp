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
    registry.add_component(entity, RealEngine::Acceleration{-20.0f, 0.0f, 0.5f});
    goStraightConstant(registry, entity, deltaTime);
}

static void adjustSpeedAndRush(RealEngine::Registry& registry, RealEngine::Entity entity,
                               float deltaTime) {
    registry.add_component(entity, RealEngine::Acceleration{60.0f, 30.0f, 0.5f});
    rushAndAimTowardsTargetZigzagging(registry, entity, deltaTime);
}

EyeBomber::EyeBomber(RealEngine::Registry& registry, sf::Vector2f position)
    : _entity(registry.spawn_entity()) {
    registry.add_component(_entity, RealEngine::Position{position.x, position.y});
    registry.add_component(_entity, RealEngine::Drawable{});
    auto spriteSheet = *RealEngine::AssetManager::getInstance().getSpriteSheet("eye_bomber");
    registry.add_component(_entity, RealEngine::SpriteSheet{spriteSheet});
    registry.add_component(_entity, RealEngine::Velocity{0.0f, 0.0f, {135.0f, 135.0f}, 0.8f});
    registry.add_component(_entity, RealEngine::Acceleration{60.0f, 30.0f, 0.5f});
    registry.add_component(_entity, RealEngine::Rotation{180.0f});
    registry.add_component(_entity,
                           RealEngine::Collision{{0.0f, 0.0f, 15.f * GAME_SCALE, 10.f * GAME_SCALE},
                                                 "mob",
                                                 false,
                                                 RealEngine::CollisionType::ENEMY,
                                                 takesDamage});

    registry.add_component(_entity, RealEngine::Health{15, 15});
    registry.add_component(_entity, RealEngine::TargetRadius{300.0f});
    registry.add_component(_entity, RealEngine::Score{30});
    registry.add_component(_entity, RealEngine::AI{adjustSpeedAndRush, simpleBehavior, true});
    registry.add_component(_entity, RealEngine::Damage{15});
    registry.add_component(_entity, RealEngine::AutoDestructible{-1.0f, true, false});
    registry.add_component(
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
    auto* acceleration = registry.get_component<RealEngine::Acceleration>(_entity);

    registry.add_component(_entity, RealEngine::Target{target});
}

}  // namespace rtype