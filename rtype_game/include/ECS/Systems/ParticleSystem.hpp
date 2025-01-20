/*
** EPITECH PROJECT, 2024
** r-teh
** File description:
** ParticleSystem
*/

#ifndef PARTICLESYSTEM_HPP_
#define PARTICLESYSTEM_HPP_

#pragma once

#include "ECS/Components/Particle.hpp"
#include "ECS/Components/ParticleEmitter.hpp"
#include "ECS/Registry/Registry.hpp"

namespace RealEngine {
class ParticleSystem {
   public:
    void update(Registry& registry, float deltaTime);
};
}  // namespace RealEngine

#endif /* !PARTICLESYSTEM_HPP_ */
