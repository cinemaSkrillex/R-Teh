/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** interpolation
*/

#pragma once

/**
 * @namespace LagCompensation
 * @brief Contains functions and utilities for lag compensation.
 */

namespace LagCompensation {

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
}  // namespace LagCompensation