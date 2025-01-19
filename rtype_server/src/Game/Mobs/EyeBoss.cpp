/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** EyeBoss
*/

#include "Game/Mobs/EyeBoss.hpp"

namespace rtype {

static void circularAttack(RealEngine::Registry& registry, RealEngine::Entity entity,
                           float deltaTime) {
    auto* rotation      = registry.get_component<RealEngine::Rotation>(entity);
    auto* spriteSheet   = registry.get_component<RealEngine::SpriteSheet>(entity);
    auto* container     = registry.get_component<RealEngine::NetvarContainer>(entity);
    auto* position      = registry.get_component<RealEngine::Position>(entity);
    float shootCooldown = 0.f;

    if (container) {
        auto* netvar = container->getNetvar("shootCooldown");
        if (netvar && netvar->value.type() == typeid(float)) {
            shootCooldown = std::any_cast<float>(netvar->value);
        }
    }

    if (spriteSheet) {
        spriteSheet->spriteIndex = "long";
        spriteSheet->frameSize   = {81, 55};
    }

    if (rotation && position) {
        rotation->angle = std::fmod(rotation->angle, 360.f);
        float angleRad  = rotation->angle * M_PI / 180.0f;
        rotation->angle += 200.0f * deltaTime;
        if (shootCooldown <= 0) {
            EyeLaser laser(registry,
                           {static_cast<float>(position->x + 20.0f * cos(angleRad)),
                            static_cast<float>(position->y + 20.0f * sin(angleRad))},
                           rotation->angle);
            shootCooldown = 0.08f;
        }
        shootCooldown -= deltaTime;

        if (container) {
            auto* netvar = container->getNetvar("shootCooldown");
            if (netvar && netvar->value.type() == typeid(float)) {
                try {
                    netvar->value = shootCooldown;
                } catch (const std::bad_any_cast& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                }
            }
        }
    }
}

static void shortRangeBehavior(RealEngine::Registry& registry, RealEngine::Entity& entity,
                               RealEngine::Entity target) {
    auto* position       = registry.get_component<RealEngine::Position>(entity);
    auto* acceleration   = registry.get_component<RealEngine::Acceleration>(entity);
    auto* velocity       = registry.get_component<RealEngine::Velocity>(entity);
    auto* targetPosition = registry.get_component<RealEngine::Position>(target);

    if (position && targetPosition && acceleration) {
        float dx       = targetPosition->x - position->x;
        float dy       = targetPosition->y - position->y;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance > 10.0f) {
            acceleration->ax = dx / distance * 6.0f;
            acceleration->ay = dy / distance * 6.0f;
            velocity->vx += acceleration->ax;
            velocity->vy += acceleration->ay;
        } else {
            acceleration->ax = 0.0f;
            acceleration->ay = 0.0f;
        }
    }
}

static void midRangeBehavior(RealEngine::Registry& registry, RealEngine::Entity& entity,
                             RealEngine::Entity target) {
    auto* position     = registry.get_component<RealEngine::Position>(entity);
    auto* acceleration = registry.get_component<RealEngine::Acceleration>(entity);
    auto* velocity     = registry.get_component<RealEngine::Velocity>(entity);
    auto* rotation     = registry.get_component<RealEngine::Rotation>(entity);
    auto* container    = registry.get_component<RealEngine::NetvarContainer>(entity);

    if (abs(velocity->vx) < 25.0f && abs(velocity->vy) < 25.0f) {
        if (position && acceleration && rotation) {
            float angleRad   = rotation->angle * M_PI / 180.0f;
            acceleration->ax = std::cos(angleRad) * 250.0f;
            acceleration->ay = std::sin(angleRad) * 250.0f;
            velocity->vx += acceleration->ax;
            velocity->vy += acceleration->ay;
        }
    }
}

static void targetBossBehavior(RealEngine::Registry& registry, RealEngine::Entity entity,
                               float deltaTime) {
    auto  target        = registry.get_component<RealEngine::Target>(entity);
    float rotationSpeed = 0.1f;
    auto* container     = registry.get_component<RealEngine::NetvarContainer>(entity);
    float shootCooldown = std::any_cast<float>(container->getNetvar("shootCooldown")->value);
    float shootPhase    = std::any_cast<float>(container->getNetvar("shootPhaseTimer")->value);
    int   bossState     = std::any_cast<int>(container->getNetvar("boss_state")->value);
    auto* spriteSheet   = registry.get_component<RealEngine::SpriteSheet>(entity);

    if (bossState == 0 && spriteSheet) {
        spriteSheet->spriteIndex = "short";
        spriteSheet->frameSize   = {73, 55};
        shortRangeBehavior(registry, entity, *(target->target));
        rotationSpeed = 1.0f;
    } else if (bossState == 1 && spriteSheet) {
        spriteSheet->spriteIndex = "mid";
        spriteSheet->frameSize   = {91, 55};
        midRangeBehavior(registry, entity, *(target->target));
        rotationSpeed = 0.6f;
    }
    aimAtTarget(entity, registry.get_component<RealEngine::Position>(*(target->target)), registry,
                rotationSpeed, deltaTime);
}

static void setBossStatusShort(RealEngine::Registry& registry, RealEngine::Entity entity) {
    auto* container = registry.get_component<RealEngine::NetvarContainer>(entity);

    try {
        container->getNetvar("boss_state")->value = 0;
    } catch (const std::bad_any_cast& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

static void setBossStatusMid(RealEngine::Registry& registry, RealEngine::Entity entity) {
    auto* spriteSheet = registry.get_component<RealEngine::SpriteSheet>(entity);
    auto* container   = registry.get_component<RealEngine::NetvarContainer>(entity);

    try {
        container->getNetvar("boss_state")->value = 1;
    } catch (const std::bad_any_cast& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

static void updateShootCooldown(RealEngine::Registry& registry, RealEngine::Entity entity,
                                RealEngine::Netvar& currentNetvar, float deltaTime) {
    auto* target = registry.get_component<RealEngine::Target>(entity);
    if (!target) return;

    auto* container = registry.get_component<RealEngine::NetvarContainer>(entity);
    auto* position  = registry.get_component<RealEngine::Position>(entity);
    auto* rotation  = registry.get_component<RealEngine::Rotation>(entity);
    float cooldown  = std::any_cast<float>(currentNetvar.value);
    int   bossState = std::any_cast<int>(container->getNetvar("boss_state")->value);

    if (cooldown <= 0.0f && bossState == 1 && rotation && position) {
        if (rand() % 3 <= 1) {
            float    angleRad = rotation->angle * M_PI / 180.0f;
            EyeLaser laser(registry,
                           {position->x + 20.0f * std::cos(angleRad),
                            position->y + 20.0f * std::sin(angleRad)},
                           rotation->angle);
        }
        cooldown = 0.75f;
    }
    cooldown -= deltaTime;
    currentNetvar.value = cooldown;
}

static void updateShootPhaseTimer(RealEngine::Registry& registry, RealEngine::Entity entity,
                                  RealEngine::Netvar& currentNetvar, float deltaTime) {
    auto* target = registry.get_component<RealEngine::Target>(entity);
    if (!target) return;

    auto* container     = registry.get_component<RealEngine::NetvarContainer>(entity);
    auto* position      = registry.get_component<RealEngine::Position>(entity);
    auto* rotation      = registry.get_component<RealEngine::Rotation>(entity);
    float cooldown      = std::any_cast<float>(currentNetvar.value);
    float shootCooldown = std::any_cast<float>(container->getNetvar("shootCooldown")->value);

    if (cooldown > 0.0f) {
        cooldown -= deltaTime;
        if (shootCooldown <= 3.2f) {
            try {
                container->getNetvar("shootCooldown")->value = 0.2f;
            } catch (const std::bad_any_cast& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        } else {
            cooldown = 20.0f;
        }
        currentNetvar.value = cooldown;
    }
}

static void doNothing(RealEngine::Registry& registry, RealEngine::Entity entity) {
    (void)registry;
    (void)entity;
}

EyeBoss::EyeBoss(RealEngine::Registry& registry, sf::Vector2f position)
    : _entity(registry.spawn_entity()) {
    registry.add_components(_entity, RealEngine::Position{position.x, position.y});
    registry.add_component(_entity, RealEngine::Velocity{0.0f, 0.0f, {300.0f, 300.0f}, 1.8f});
    registry.add_component(_entity, RealEngine::Acceleration{2.0f, 2.0f, 2.0f});
    registry.add_component(
        _entity, RealEngine::Interpolation{
                     {position.x, position.y}, {position.x, position.y}, 0.f, 1.f, false});
    auto spriteSheet = *RealEngine::AssetManager::getInstance().getSpriteSheet("eye_boss");
    registry.add_component(_entity, RealEngine::SpriteSheet{spriteSheet});
    registry.add_component(_entity, RealEngine::Drawable{});
    registry.add_component(_entity, RealEngine::Rotation{180.0f});
    registry.add_component(_entity,
                           RealEngine::Collision{{0.0f, 0.0f, 15.f * GAME_SCALE, 10.f * GAME_SCALE},
                                                 "mob",
                                                 false,
                                                 RealEngine::CollisionType::ENEMY,
                                                 takesDamage});
    registry.add_component(_entity, RealEngine::Health{1000, 1000});
    registry.add_component(_entity, RealEngine::Damage{20});
    registry.add_component(_entity, RealEngine::Radius{800.0f, 200.0f, setBossStatusMid, doNothing,
                                                       setBossStatusShort});
    registry.add_component(_entity, RealEngine::AI{targetBossBehavior, circularAttack, true});
    registry.add_component(_entity, RealEngine::TargetRadius{500.0f, false});
    registry.add_component(
        _entity,
        RealEngine::NetvarContainer{{
            {"sprite_name", {"string", "sprite_name", std::string("eye_boss"), nullptr}},
            {"new_entity", {"bool", "new_entity", true, nullptr}},
            {"entity_type", {"int", "entity_type", 2, nullptr}},
            {"shootCooldown", {"float", "shootCooldown", float(0.f), updateShootCooldown}},
            {"shootPhaseTimer", {"float", "shootPhaseTimer", float(0.f), updateShootPhaseTimer}},
            {"boss_state", {"int", "boss_state", 0, nullptr}},

        }});
}

EyeBoss::~EyeBoss() {}

void EyeBoss::setTarget(RealEngine::Registry&               registry,
                        std::shared_ptr<RealEngine::Entity> target) {
    registry.add_component(_entity, RealEngine::Target{target});
}

}  // namespace rtype