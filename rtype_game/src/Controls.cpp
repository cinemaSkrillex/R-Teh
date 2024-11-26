#include "Controls.hpp"

namespace rtype {
void Controls::moveUp(Velocity& velocity, Acceleration& acceleration, float deltaTime) {
    if (velocity.vy > 50)
        velocity.vy = 50;
    velocity.vy -= acceleration.ay * 3 * deltaTime;
    if (velocity.vy < -400)
        velocity.vy = -400;
}

void Controls::moveDown(Velocity& velocity, Acceleration& acceleration, float deltaTime) {
    if (velocity.vy < -50)
        velocity.vy = -50;
    velocity.vy += acceleration.ay * 3 * deltaTime;
    if (velocity.vy > 400)
        velocity.vy = 400;
}

void Controls::moveLeft(Velocity& velocity, Acceleration& acceleration, float deltaTime) {
    if (velocity.vx > 50)
        velocity.vx = 50;
    velocity.vx -= acceleration.ax * 3 * deltaTime;
    if (velocity.vx < -400)
        velocity.vx = -400;
}

void Controls::moveRight(Velocity& velocity, Acceleration& acceleration, float deltaTime) {
    if (velocity.vx < -50)
        velocity.vx = -50;
    velocity.vx += acceleration.ax * 3 * deltaTime;
    if (velocity.vx > 400)
        velocity.vx = 400;
}

void Controls::decelerateUp(Velocity& velocity, Acceleration& acceleration, float deltaTime) {
    if (velocity.vy < 0) {
        velocity.vy += acceleration.ay * 3 * deltaTime;
        if (velocity.vy > 0)
            velocity.vy = 0;
    }
}

void Controls::decelerateDown(Velocity& velocity, Acceleration& acceleration, float deltaTime) {
    if (velocity.vy > 0) {
        velocity.vy -= acceleration.ay * 3 * deltaTime;
        if (velocity.vy < 0)
            velocity.vy = 0;
    }
}

void Controls::decelerateLeft(Velocity& velocity, Acceleration& acceleration, float deltaTime) {
    if (velocity.vx < 0) {
        velocity.vx += acceleration.ax * 3 * deltaTime;
        if (velocity.vx > 0)
            velocity.vx = 0;
    }
}

void Controls::decelerateRight(Velocity& velocity, Acceleration& acceleration, float deltaTime) {
    if (velocity.vx > 0) {
        velocity.vx -= acceleration.ax * 3 * deltaTime;
        if (velocity.vx < 0)
            velocity.vx = 0;
    }
}
} // namespace rtype