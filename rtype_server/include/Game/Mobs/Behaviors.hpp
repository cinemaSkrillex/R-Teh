/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** Repetitive behaviors for several mobs
*/

#pragma once

#include <cmath>

#include "Engine.hpp"

namespace rtype {
// AI

void noBehavior(RealEngine::Registry& registry, RealEngine::Entity entity, float deltaTime);
void aimAtTarget(RealEngine::Entity& entity, RealEngine::Position* targetPosition,
                 RealEngine::Registry& registry, float rotationSpeed, float deltaTime);
void rushAndAimTowardsTarget(RealEngine::Registry& registry, RealEngine::Entity entity,
                             float deltaTime);
void rushAndAimTowardsTargetZigzagging(RealEngine::Registry& registry, RealEngine::Entity entity,
                                       float deltaTime);
void rushTowardsTarget(RealEngine::Registry& registry, RealEngine::Entity entity, float deltaTime);

void goStraightAngle(RealEngine::Registry& registry, RealEngine::Entity entity, float deltaTime);
void goStraight(RealEngine::Registry& registry, RealEngine::Entity entity, float deltaTime);
void goStraightConstant(RealEngine::Registry& registry, RealEngine::Entity entity, float deltaTime);
void goStraightConstantAngle(RealEngine::Registry& registry, RealEngine::Entity entity,
                             float deltaTime);

// collisions

void noCollisionBehavior(RealEngine::CollisionType collisionType, RealEngine::Registry& registry,
                         RealEngine::Entity collider, RealEngine::Entity entity);
void destroyOnWalls(RealEngine::CollisionType collisionType, RealEngine::Registry& registry,
                    RealEngine::Entity collider, RealEngine::Entity entity);
void destroyOnWallsAndPlayer_TakesDamage(RealEngine::CollisionType collisionType,
                                         RealEngine::Registry&     registry,
                                         RealEngine::Entity collider, RealEngine::Entity entity);
void destroyOnWallsAndPlayer(RealEngine::CollisionType collisionType,
                             RealEngine::Registry& registry, RealEngine::Entity collider,
                             RealEngine::Entity entity);
void takesDamage(RealEngine::CollisionType collisionType, RealEngine::Registry& registry,
                 RealEngine::Entity collider, RealEngine::Entity entity);
void takeDamageFromCollider(RealEngine::Registry& registry, RealEngine::Entity collider,
                            RealEngine::Entity entity);
void selfDestruct(RealEngine::Registry& registry, RealEngine::Entity entity);
void blockEntity(RealEngine::Registry& registry, RealEngine::Entity collider,
                 RealEngine::Entity entity);
}  // namespace rtype