#include <SFML/Graphics.hpp>
#include "ECS/Registry/Registry.hpp"
#include "ECS/Components/Position.hpp"
#include "ECS/Components/Sprite.hpp"
#include "ECS/Components/SpriteSheet.hpp"
#include "ECS/Components/Velocity.hpp"
#include "ECS/Components/Drawable.hpp"
#include "ECS/Components/Controllable.hpp"
#include "ECS/Systems/DrawSystem.hpp"
#include "ECS/Systems/ControlSystem.hpp"
#include "ECS/Components/Acceleration.hpp"
#include "ECS/Systems/MovementSystem.hpp"
#include "Core/Window.hpp"
#include "Controls.hpp"

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
    RealEngine::Sprite upSpaceship("../assets/spaceship.png", {0, 0, 32 * 2, 15});
    RealEngine::Sprite idleSpaceship("../assets/spaceship.png", {0, 15, 32, 15});
    RealEngine::Sprite downSpaceship("../assets/spaceship.png", {0, 15 * 2, 33 * 2, 15});

    idleSpaceship.setScale(3, 3);
    upSpaceship.setScale(3, 3);
    downSpaceship.setScale(3, 3);

    std::unordered_map<std::string, RealEngine::Sprite> spaceshipSheet;
    spaceshipSheet.emplace("up", upSpaceship);
    spaceshipSheet.emplace("idle", idleSpaceship);
    spaceshipSheet.emplace("down", downSpaceship);

    // Create entities
    Entity entity1 = registry.spawn_entity();
    Entity entity2 = registry.spawn_entity();

    // There is 2 ways to add components to entities
    // 1. Add all components at once
    registry.add_components(entity1, Position{100.f, 100.f}, Drawable{});
    registry.add_component(entity1, Sprite{idleSpaceship});

    // // 2. Add components one by one
    registry.add_component(entity2, Position{200.f, 200.f});
    registry.add_component(entity2, Velocity{0.0f, 0.0f});
    registry.add_component(entity2, Acceleration{10.0f, 10.0f, 10.0f, true});
    registry.add_component(entity2, Controllable{});
    registry.add_component(entity2, Drawable{});
    registry.add_component(entity2,
                           SpriteSheet{spaceshipSheet, "idle", 0, {32, 15}, false, false, 100});

    DrawSystem drawSystem(window.getRenderWindow());

    ControlSystem   controlSystem;
    rtype::Controls controls(registry);
    controlSystem.bindKey(sf::Keyboard::Z, Action::Up);
    controlSystem.bindKey(sf::Keyboard::S, Action::Down);
    controlSystem.bindKey(sf::Keyboard::Q, Action::Left);
    controlSystem.bindKey(sf::Keyboard::D, Action::Right);
    controlSystem.bindKey(sf::Keyboard::Space, Action::Action1);
    controlSystem.setActionHandler(
        Action::Up, std::bind(&rtype::Controls::moveUp, &controls, std::placeholders::_1,
                              std::placeholders::_2, std::placeholders::_3));
    controlSystem.setActionHandler(
        Action::Down, std::bind(&rtype::Controls::moveDown, &controls, std::placeholders::_1,
                                std::placeholders::_2, std::placeholders::_3));
    controlSystem.setActionHandler(
        Action::Left, std::bind(&rtype::Controls::moveLeft, &controls, std::placeholders::_1,
                                std::placeholders::_2, std::placeholders::_3));
    controlSystem.setActionHandler(
        Action::Right, std::bind(&rtype::Controls::moveRight, &controls, std::placeholders::_1,
                                 std::placeholders::_2, std::placeholders::_3));
    controlSystem.setActionHandler(
        Action::Action1, std::bind(&rtype::Controls::shoot, &controls, std::placeholders::_1,
                                   std::placeholders::_2, std::placeholders::_3));

    controlSystem.setActionReleaseHandler(
        Action::Up, std::bind(&rtype::Controls::decelerateUp, &controls, std::placeholders::_1,
                              std::placeholders::_2, std::placeholders::_3));
    controlSystem.setActionReleaseHandler(
        Action::Down, std::bind(&rtype::Controls::decelerateDown, &controls, std::placeholders::_1,
                                std::placeholders::_2, std::placeholders::_3));
    controlSystem.setActionReleaseHandler(
        Action::Left, std::bind(&rtype::Controls::decelerateLeft, &controls, std::placeholders::_1,
                                std::placeholders::_2, std::placeholders::_3));
    controlSystem.setActionReleaseHandler(Action::Right,
                                          std::bind(&rtype::Controls::decelerateRight, &controls,
                                                    std::placeholders::_1, std::placeholders::_2,
                                                    std::placeholders::_3));
    controlSystem.setActionReleaseHandler(
        Action::Action1, std::bind(&rtype::Controls::voidAction, &controls, std::placeholders::_1,
                                   std::placeholders::_2, std::placeholders::_3));

    MovementSystem movementSystem;

    float deltaTime = 0.f;
    registry.add_system<Position, Velocity>(
        [&movementSystem](Registry& registry, float deltaTime, auto& positions, auto& velocities) {
            movementSystem.update(registry, positions, velocities, deltaTime);
        });

    registry.add_system<Position, Drawable, Sprite, SpriteSheet>(
        [&drawSystem](Registry& registry, float deltaTime, auto& positions, auto& drawables,
                      auto& sprites, auto& spritesheets) {
            drawSystem.update(registry, deltaTime, positions, drawables, sprites, spritesheets);
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