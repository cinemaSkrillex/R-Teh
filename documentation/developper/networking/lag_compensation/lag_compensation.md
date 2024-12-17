## Synchronized Movement

### Overview

The R-type server uses interpolation and delta times to achieve synchronized movement between the server and clients. 

This ensures smooth and consistent movement across all clients, even in the presence of network latency.

### Interpolation

Interpolation is used to predict the position of entities based on their previous and current positions.

The server calculates the interpolation factor and sends it to the clients, which then use it to update the positions of entities smoothly.

### Explanation of the Interpolation Process

Interpolation works by estimating the position of an entity at a given point in time based on its previous and current positions.

This is particularly useful in networked games where there may be delays in receiving position updates from the server. 

By interpolating between known positions, the client can provide smooth and continuous movement, reducing the visual impact of network latency.

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

The server calculates the delta time in the [RtypeServer::run](../../../../rtype_server/src/RtypeServer/RtypeServerRun.cpp) method:

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
                for (const auto& message : _server->get_unreliable_messages_from_endpoint(client)) {
                    const auto parsed_data = PeterParser::parseMessage(message);

                    if (parsed_data.find("Event") != parsed_data.end()) {
                        runEvent(parsed_data, client, _players.at(client));
                    } else {
                        runSimulation(parsed_data, client, _players.at(client));
                    }
                }
            }

            _game_instance->run(_deltaTime);
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

The server broadcasts the player positions in the [RtypeServer::broadcastPlayerState](../../../../rtype_server/src/RtypeServer/RtypeServerUtils.cpp) method:

```cpp
void RtypeServer::broadcastPlayerState(const Player& player) {
    // Get the player's position
    RealEngine::Entity* entity = player.getEntity();
    auto* position = _game_instance->getRegistryRef().get_component<RealEngine::Position>(*entity);
    if (position) {
        std::string message = "Event:Player_position Uuid:" + std::to_string(player.getUUID()) +
                              " Step:" + std::to_string(_deltaTimeBroadcast) + " Position:(" +
                              std::to_string(position->x) + "," + std::to_string(position->y) + ")";
        for (auto client : _server->getClients()) {
            _server->send_unreliable_packet(message, client);
        }
    }
}
```

### Game position handling

The client handles the player position updates in the [Game::handlePlayerPosition](../../../../rtype_game/src/Game/GameUtils.cpp) method:

```cpp
void rtype::Game::handlePlayerPosition(std::unordered_map<std::string, std::string> parsedPacket) {
    const long int     uuid     = std::stol(parsedPacket.at("Uuid"));
    const sf::Vector2f position = PeterParser::parseVector2f(parsedPacket.at("Position"));
    float              step     = std::stof(parsedPacket.at("Step"));
    auto               it       = _players.find(uuid);

    if (it == _players.end()) return;
    RealEngine::Entity player = it->second;
    // Update the position component of the player
    auto* positionComponent      = _registry.get_component<RealEngine::Position>(player);
    auto* interpolationComponent = _registry.get_component<RealEngine::Interpolation>(player);
    if (!positionComponent && !interpolationComponent) return;
    positionComponent->x                 = interpolationComponent->end.x;
    positionComponent->y                 = interpolationComponent->end.y;
    interpolationComponent->start        = {positionComponent->x, positionComponent->y};
    interpolationComponent->end          = position;
    interpolationComponent->step         = 1.f / step;
    interpolationComponent->current_step = 0.f;
    interpolationComponent->reset        = true;
}
```