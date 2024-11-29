# How to create a system

A system is part of the game engine ECS (Entity-Component-System) architecture. [see more](global_systems_doc.md)

## How does it work?

A system is a class that has an `update` method. The `update` method takes a `Registry` object as a parameter. The `Registry` object is a class that manages the entities and their components.

## How to create your own system ?

To create your own system, you need to create your own file into game_engine/ECS/Systems folder.

Let's create a simple system that prints the position of the entities with a `Movement` component.

```cpp
void MovementSystem::update(Registry& registry, float deltaTime) {
    auto entities = registry.view<Position, Velocity>();

    for (auto entity : entities) {
        auto* position = registry.get_component<Position>(entity);
        auto* velocity = registry.get_component<Velocity>(entity);
        if (position && velocity) {
            position->x += velocity->vx * deltaTime;
            position->y += velocity->vy * deltaTime;
        }
    }
}
```

Let's explain the code above:

- `auto entities = registry.view<Position, Velocity>();` : This line gets all the entities that have a `Position` and `Velocity` component.

- `for (auto entity : entities) {` : This line iterates over all the entities.

- `auto* position = registry.get_component<Position>(entity);` : This line gets the `Position` component of the entity.

- `auto* velocity = registry.get_component<Velocity>(entity);` : This line gets the `Velocity` component of the entity.

- `if (position && velocity) {` : This line checks if the entity has a `Position` and `Velocity` component.

- `position->x += velocity->vx * deltaTime;` : This line updates the x position of the entity.

- `position->y += velocity->vy * deltaTime;` : This line updates the y position of the entity.

## How to use your system ?

TODO