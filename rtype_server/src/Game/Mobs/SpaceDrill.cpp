/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** SpaceDrill
*/

#include "Game/Mobs/SpaceDrill.hpp"

namespace rtype {

static void waitingForDash(RealEngine::Registry& registry, RealEngine::Entity entity,
                           float deltaTime) {
    auto* container = registry.get_component<RealEngine::NetvarContainer>(entity);

    if (container) {
        auto* spriteSheet   = registry.get_component<RealEngine::SpriteSheet>(entity);
        auto* velocity      = registry.get_component<RealEngine::Velocity>(entity);
        auto* acceleration  = registry.get_component<RealEngine::Acceleration>(entity);
        float autoDashTimer = std::any_cast<float>(container->getNetvar("autoDashTimer")->value);
        if (autoDashTimer <= 0) {
            spriteSheet->pause = false;
            velocity->vx       = acceleration->ax;
        }
    }
}

static void alignOnTargetOnY(RealEngine::Registry& registry, RealEngine::Entity entity,
                             float deltaTime) {
    auto  entity_target  = registry.get_component<RealEngine::Target>(entity);
    auto* position       = registry.get_component<RealEngine::Position>(entity);
    auto* velocity       = registry.get_component<RealEngine::Velocity>(entity);
    auto* targetPosition = registry.get_component<RealEngine::Position>(entity_target->target);
    auto* acceleration   = registry.get_component<RealEngine::Acceleration>(entity);

    if (position && targetPosition) {
        float dy = targetPosition->y - position->y;

        if (std::abs(dy) > 1.0f) {
            velocity->vy = (dy > 0) ? std::min(acceleration->ay, dy / deltaTime)
                                    : std::max(-acceleration->ay, dy / deltaTime);
        } else {
            velocity->vy = 0;
        }
    }
    waitingForDash(registry, entity, deltaTime);
}

static void moveUpAndDown(RealEngine::Registry& registry, RealEngine::Entity entity,
                          float deltaTime) {
    auto* container = registry.get_component<RealEngine::NetvarContainer>(entity);
    auto* position  = registry.get_component<RealEngine::Position>(entity);
    bool  goUp      = std::any_cast<bool>(container->getNetvar("oscillationDirection")->value);

    if (position) {
        if (goUp) {
            position->y -= 50.f * deltaTime;
        } else {
            position->y += 50.f * deltaTime;
        }
    }
    waitingForDash(registry, entity, deltaTime);
}

static void oscillationTimerUpdate(RealEngine::Registry& registry, RealEngine::Entity entity,
                                   RealEngine::Netvar& currentNetvar, float deltaTime) {
    auto* container = registry.get_component<RealEngine::NetvarContainer>(entity);
    auto* position  = registry.get_component<RealEngine::Position>(entity);
    float cooldown  = std::any_cast<float>(currentNetvar.value);
    bool  goUp      = std::any_cast<bool>(container->getNetvar("oscillationDirection")->value);

    if (cooldown <= 0) {
        cooldown                                            = 4.f;
        goUp                                                = !goUp;
        container->getNetvar("oscillationDirection")->value = goUp;
    }
    cooldown -= deltaTime;
    currentNetvar.value = cooldown;
}

static void updateAutoDashTimer(RealEngine::Registry& registry, RealEngine::Entity entity,
                                RealEngine::Netvar& currentNetvar, float deltaTime) {
    auto* container     = registry.get_component<RealEngine::NetvarContainer>(entity);
    float autoDashTimer = std::any_cast<float>(currentNetvar.value);

    if (autoDashTimer <= 0) {
        autoDashTimer = 0.f;
    }
    autoDashTimer -= deltaTime;
    currentNetvar.value = autoDashTimer;
}
SpaceDrill::SpaceDrill(RealEngine::Registry& registry, sf::Vector2f position)
    : _entity(registry.spawn_entity()),
      _mobSprite(*(RealEngine::AssetManager::getInstance().getSprite("space_drill"))) {
    _mobSpriteSheet.emplace("normal", _mobSprite);
    registry.add_component(_entity, RealEngine::Position{position.x, position.y});
    registry.add_component(
        _entity, RealEngine::Interpolation{
                     {position.x, position.y}, {position.x, position.y}, 0.f, 1.f, false});
    registry.add_component(_entity, RealEngine::Velocity{0, 0, {550.f, 550.f}, 0.5f});
    registry.add_component(_entity, RealEngine::Acceleration{-1600.0f, 70.0f, 0.5f});
    registry.add_component(
        _entity,
        RealEngine::SpriteSheet{
            _mobSpriteSheet, "normal", 0, {32, 22}, true, true, 40, {16, 11}, sf::Clock()});
    registry.add_component(_entity, RealEngine::Drawable{});
    registry.add_component(_entity,
                           RealEngine::Collision{{0.f, 0.f, 16.f * GAME_SCALE, 8.f * GAME_SCALE},
                                                 "mob",
                                                 false,
                                                 RealEngine::CollisionType::ENEMY,
                                                 takesDamage});
    registry.add_component(_entity, RealEngine::AI{alignOnTargetOnY, moveUpAndDown, true});
    registry.add_component(_entity, RealEngine::TargetRadius{400.0f});
    registry.add_component(_entity, RealEngine::Damage{20});
    registry.add_component(_entity, RealEngine::Health{10, 10});
    registry.add_component(_entity, RealEngine::Rotation{0.f});
    registry.add_component(_entity, RealEngine::AutoDestructible{-1.0f, true, false});
    registry.add_component(
        _entity,
        RealEngine::NetvarContainer{{
            {"sprite_name", {"string", "sprite_name", std::string("space_drill"), nullptr}},
            {"new_entity", {"bool", "new_entity", true, nullptr}},
            {"destroy_out_of_screen", {"bool", "destroy_out_of_screen", false, nullptr}},
            {"autoDashTimer", {"float", "autoDashTimer", 15.f, updateAutoDashTimer}},
            {"oscillationTimer", {"float", "oscillationTimer", 0.0f, oscillationTimerUpdate}},
            {"oscillationDirection", {"bool", "oscillationDirection", true, nullptr}},
        }});
}

SpaceDrill::~SpaceDrill() {}

}  // namespace rtype