/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** LagCompensation
*/

#include "ECS/Systems/LagCompensationSystem.hpp"

namespace RealEngine {
LagCompensationSystem::LagCompensationSystem() {}

LagCompensationSystem::~LagCompensationSystem() {}

void LagCompensationSystem::update(Registry& registry, float deltaTime) {
    auto entities = registry.view<Interpolation, Position>();

    if (entities.empty()) return;
    for (auto entity : entities) {
        Interpolation* interpolation = registry.getComponent<Interpolation>(entity);
        Position*      position      = registry.getComponent<Position>(entity);
        if (interpolation->reset) {
            position->x          = interpolation->end.x;
            position->y          = interpolation->end.y;
            interpolation->reset = false;
        }
        sf::Vector2f newPos = LagCompensation::lerp(interpolation->start, interpolation->end,
                                                    interpolation->currentStep);
        position->x         = newPos.x;
        position->y         = newPos.y;

        interpolation->currentStep =
            std::min(interpolation->currentStep + interpolation->step * deltaTime, 1.0f);
    }
}

/**
 * @brief Linearly interpolates between two values.
 *
 * This function performs a linear interpolation between two values `a` and `b`
 * based on the interpolation factor `t`. The interpolation factor `t` should
 * be in the range [0, 1], where 0 returns `a` and 1 returns `b`. Values of `t`
 * outside this range will result in extrapolation.
 *
 * @tparam T The type of the values to interpolate.
 * @param a The start value.
 * @param b The end value.
 * @param t The interpolation factor.
 * @return The interpolated value.
 */
template <typename T>
T lerp(const T& a, const T& b, float t) {
    return a + t * (b - a);
}

/**
 * @brief Calculates the inverse linear interpolation factor.
 *
 * This function calculates the interpolation factor `t` given two values `a`
 * and `b`, and a value `v` that lies between `a` and `b`. The interpolation
 * factor `t` will be in the range [0, 1] if `v` is between `a` and `b`.
 * Values of `v` outside this range will result in a factor outside [0, 1].
 *
 * @tparam T The type of the values to interpolate.
 * @param a The start value.
 * @param b The end value.
 * @param v The value for which to calculate the interpolation factor.
 * @return The interpolation factor.
 */
template <typename T>
T invlerp(const T& a, const T& b, const T& v) {
    if (a == b) {
        return 0.0f;
    }
    return (v - a) / (b - a);
}

}  // namespace RealEngine