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
    return (v - a) / (b - a);
}

}  // namespace LagCompensation