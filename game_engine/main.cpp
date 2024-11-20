#include <SFML/Graphics.hpp>
#include "../include/ECS/Registry/Registry.hpp"
#include "../include/ECS/Components/Position.hpp"
#include "../include/ECS/Components/Sprite.hpp"
#include "../include/ECS/Components/SpriteSheet.hpp"
#include "../include/ECS/Components/Velocity.hpp"
#include "../include/ECS/Components/Drawable.hpp"
#include "../include/ECS/Components/Controllable.hpp"
#include "../include/ECS/Systems/DrawSystem.hpp"
#include "../include/ECS/Systems/ControlSystem.hpp"
#include "../include/ECS/Components/Acceleration.hpp"
#include "../include/ECS/Systems/MovementSystem.hpp"

#include "../include/Core/Window.hpp"

int main() {
    RealEngine::Window window("SKRILLEX", sf::Vector2u(800, 600));

    Registry registry;

    // Register components
    registry.register_component<Position>();
    registry.register_component<Velocity>();
    registry.register_component<Sprite>();
    registry.register_component<SpriteSheet>();
    registry.register_component<Drawable>();
    registry.register_component<Controllable>();
    registry.register_component<Acceleration>();

    // Sprites
    RealEngine::Sprite spaceship("../assets/spaceship.png");
    RealEngine::Sprite spaceship2("../assets/spaceship.png", {0, 0, 32, 15});


    // Create entities
    Entity entity1 = registry.spawn_entity();
    Entity entity2 = registry.spawn_entity();

    // There is 2 ways to add components to entities
    // 1. Add all components at once
    registry.add_components(entity1, Position{100.f, 100.f}, Drawable{});
    registry.add_component(entity1, SpriteSheet{Sprite{spaceship}});

    // // 2. Add components one by one
    registry.add_component(entity2, Position{200.f, 200.f});
    registry.add_component(entity2, Velocity{0.f, 0.f});
    registry.add_component(entity2, Acceleration{0.1f, 0.1f, 0.f, true});
    registry.add_component(entity2, Controllable{});
    registry.add_component(entity2, Drawable{});
    registry.add_component(entity2, Sprite{spaceship2});

    DrawSystem     drawSystem(window.getRenderWindow());
    ControlSystem  controlSystem;
    MovementSystem movementSystem;

    float deltaTime = 0.f;
    registry.add_system<Position, Velocity>(
        [&movementSystem](Registry& registry, float deltaTime, auto& positions, auto& velocities) {
            movementSystem.update(registry, positions, velocities, deltaTime);
        });

    registry.add_system<Position, Drawable, Sprite>([&drawSystem](Registry& registry,
                                                                  float deltaTime, auto& positions,
                                                                  auto& drawables, auto& sprites) {
        drawSystem.update(registry, deltaTime, positions, drawables, sprites);
    });

    registry.add_system<Velocity, Controllable, Acceleration>(
        [&controlSystem](Registry& registry, float deltaTime, auto& velocities, auto& controllables,
                         auto& accelerations) {
            controlSystem.update(registry, velocities, controllables, accelerations, deltaTime);
        });

    sf::Clock clock;

    while (window.isOpen()) {
        window.update();
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