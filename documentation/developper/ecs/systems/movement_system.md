# Movement System

The movement system is responsible for updating the entities with a `Velocity` and `Position` component based on the velocity.

It is part of the game engine ECS (Entity-Component-System) architecture. [see more](global_systems_doc.md)

## How does it work?

The movement system takes a `Registry` object as a parameter. The `Registry` object is a class that manages the entities and their components.

It then gets the entities with a `Velocity` and `Position` component from the registry and updates their position based on the velocity.

```cpp
 void update(Registry& registry) {
        auto& velocities = registry.get_components<Velocity>();
        auto& positions  = registry.get_components<Position>();
```

## What is the goal of this system ?

The goal of the movement system is to update the entities with a `Velocity` and `Position` component based on the velocity.