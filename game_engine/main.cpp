#include <SFML/Graphics.hpp>
#include "../include/ECS/Registry/Registry.hpp"
#include "../include/ECS/Components/Position.hpp"
#include "../include/ECS/Components/Velocity.hpp"
#include "../include/ECS/Components/Drawable.hpp"
#include "../include/ECS/Components/Controllable.hpp"
#include "../include/ECS/Systems/DrawSystem.hpp"
#include "../include/ECS/Systems/ControlSystem.hpp"
#include "../include/ECS/Components/Acceleration.hpp"
#include "../include/ECS/Systems/MovementSystem.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

    Registry registry;

    // Register components
    registry.register_component<Position>();
    registry.register_component<Velocity>();
    registry.register_component<Drawable>();
    registry.register_component<Controllable>();
    registry.register_component<Acceleration>();

    // Create entities
    Entity entity1 = registry.spawn_entity();
    Entity entity2 = registry.spawn_entity();

    // There is 2 ways to add components to entities
    // 1. Add all components at once
    registry.add_components(entity1, Position{100.0f, 100.0f}, Velocity{0.0f, 0.0f}, Drawable{},
                            Controllable{}, Acceleration{0.1f, 0.1f, 0.0f, true});
    // // 2. Add components one by one
    registry.add_component(entity2, Position{200.0f, 200.0f});
    registry.add_component(entity2, Drawable{});
    registry.add_component(entity2, Controllable{});

    DrawSystem     drawSystem(window);
    ControlSystem  controlSystem;
    MovementSystem movementSystem;

    float deltaTime = 0.0f;
    registry.add_system<Position, Velocity>(
        [&movementSystem](Registry& registry, float deltaTime, auto& positions, auto& velocities) {
            movementSystem.update(registry, positions, velocities, deltaTime);
        });

    registry.add_system<Position, Drawable>(
        [&drawSystem](Registry& registry, float deltaTime, auto& positions, auto& drawables) {
            drawSystem.update(registry, positions, drawables, deltaTime);
        });

    registry.add_system<Velocity, Controllable, Acceleration>(
        [&controlSystem](Registry& registry, float deltaTime, auto& velocities, auto& controllables,
                         auto& accelerations) {
            controlSystem.update(registry, velocities, controllables, accelerations, deltaTime);
        });

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        deltaTime = clock.restart().asSeconds();

        // drawSystem.update(registry);
        // controlSystem.update(registry);
        window.clear();
        registry.run_systems(deltaTime);
        // drawSystem.update(registry, deltaTime);
        window.display();
    }

    return 0;
}