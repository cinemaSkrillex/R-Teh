# Draw System

The draw system is responsible for drawing the entities with a Position and Drawable component.

It is part of the game engine ECS (Entity-Component-System) architecture. [see more](global_systems_doc.md)

## How does it work?

The draw system takes a `Registry` object as a parameter. The `Registry` object is a class that manages the entities and their components.

It then gets the `Position` and `Drawable` components from the registry and iterates over them to draw the entities.

```cpp
void update(Registry& registry) {
        auto& positions = registry.get_components<Position>();
        auto& drawables = registry.get_components<Drawable>();
        //...
```

## What is the goal of this system ?

The goal of the draw system is to render the entities with a `Position` and `Drawable` component.