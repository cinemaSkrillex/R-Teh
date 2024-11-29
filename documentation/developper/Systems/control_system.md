# Control System

The control system is responsible for updating the entities with a `Control` component.

It is part of the game engine ECS (Entity-Component-System) architecture. [see more](global_systems_doc.md)

## How does it work?

The control system takes a `Registry` object as a parameter. The `Registry` object is a class that manages the entities and their components.

It then gets the entities with a `Velocity` and `Controllable` component from the registry and updates their position based on the input.

```cpp
 void update(Registry& registry) {
        auto& velocities    = registry.get_components<Velocity>();
        auto& controllables = registry.get_components<Controllable>();
```

## More on the logic :

This system has quite more logic than for example a `DrawSystem` or a `MovementSystem`. It has to handle the input and update the entities accordingly.

This system implements a way to handle the input, bind a key to a specific action, and update the entities based on the input. For example, if the player presses the `W` key, the entity will move up.

## How does it work in practice ? 

We use a `std::unordered_map` to store the actions and their corresponding handlers. The handler is a lambda function that takes a `Velocity` reference as a parameter and updates it based on the action.

```cpp
    actionHandlers[Action::MoveUp] = [](Velocity& velocity) {
        velocity.y = -1.0f;
    };
```

We then iterate over the entities with a `Controllable` component and check if the action is pressed. If it is, we call the corresponding handler.

```cpp
void update(Registry& registry) {
        auto& velocities    = registry.get_components<Velocity>();
        auto& controllables = registry.get_components<Controllable>();

        for (std::size_t i = 0; i < controllables.size(); ++i) {
            if (controllables[i] && velocities[i]) {
                velocities[i]->vx = 0.0f;
                velocities[i]->vy = 0.0f;

                for (const auto& [key, action] : keyBindings) {
                    if (sf::Keyboard::isKeyPressed(key)) {
                        actionHandlers[action](*velocities[i]);
                    }
                }
            }
        }
    }
```

The handler functions are defined as follows:

```cpp
 void handleMoveUp(Velocity& velocity) { velocity.vy = -10.0f; }

    void handleMoveDown(Velocity& velocity) { velocity.vy = 10.0f; }
```



## What is the goal of this system ?

The goal of the control system is to update the entities with a `Velocity` and `Controllable` component based on the input.