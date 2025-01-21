---
title: Lag Compensation
parent: Networking
grand_parent: Developper
nav_order: 3
---
## Synchronized Movement

### Overview

The R-type server uses interpolation and delta times to achieve synchronized movement between the server and clients. 

This ensures smooth and consistent movement across all clients, even in the presence of Network latency.

### Interpolation

Interpolation is used to predict the position of entities based on their previous and current positions.

The server calculates the interpolation factor and sends it to the clients, which then use it to update the positions of entities smoothly.

### Explanation of the Interpolation Process

Interpolation works by estimating the position of an entity at a given point in time based on its previous and current positions.

This is particularly useful in networked games where there may be delays in receiving position updates from the server. 

By interpolating between known positions, the client can provide smooth and continuous movement, reducing the visual impact of Network latency.

#### Interpolation Function

The interpolation function is defined in the `interpolation.hpp` file:

```cpp
namespace LagCompensation {

/**
 * @brief Linearly interpolates between two values.
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

} // namespace LagCompensation
```

### Delta time 

Delta time is used to calculate the time elapsed between updates. This ensures that the movement of entities is consistent regardless of the frame rate.

#### Delta Time Calculation

The server calculates the delta time in the [RtypeServer::run](../../../../RtypeServer/src/RtypeServer/RtypeServerRun.cpp) method:

```cpp
void RtypeServer::run() {
    auto log = std::make_shared<Log>("RtypeServer.log");

    while (true) {
        if (_clock.getElapsedTime().asMilliseconds() > 1000 / _config.SERVER_TICK) {
            // Reset the clock for the next tick
            _deltaTime = _clock.restart().asSeconds();

            // Do server work
            for (auto client : _server->getClients()) {
                // Process all messages from the client
                for (const auto& message : _server->getUnreliableMessagesFromEndpoint(client)) {
                    const auto parsed_data = PeterParser::parseMessage(message);

                    if (parsed_data.find("Event") != parsed_data.end()) {
                        runEvent(parsed_data, client, _players.at(client));
                    } else {
                        runSimulation(parsed_data, client, _players.at(client));
                    }
                }
            }

            _gameInstance->run(_deltaTime);
        }
        if (_broadcastClock.getElapsedTime().asMilliseconds() >
            1000 / _config.SERVER_BROADCAST_TICK) {
            _deltaTimeBroadcast = _broadcastClock.restart().asSeconds();
            for (const auto& player : _players) {
                broadcastPlayerState(player.second);
            }
        }
    }
}
```

#### Player Position Update

The server broadcasts the updated positions of players to all clients. The clients then use interpolation to smoothly update the positions of entities.

#### Player Position Broadcast

TODO: with protocol

### Game position handling

TODO: with protocol