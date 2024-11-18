# System docs

## What is a system?

A system is a collection of components that work together to achieve a common goal. Systems are used to manage the game state, update the game state, and render the game state.

It is part of the game engine ECS (Entity-Component-System) architecture.

## How does it work?

A system is a class managing a specific aspect of the game. It is responsible for updating the game state by iterating over the components of the entities it is interested in.

## How to create a system?

To create a system, you need to create a new class in a header file, let's take for example the DrawSystem:

```cpp

class DrawSystem {
  public:
    DrawSystem(sf::RenderWindow& window) : _window(window) {}

    void update(Registry& registry) {
        auto& positions = registry.get_components<Position>();
        auto& drawables = registry.get_components<Drawable>();

        for (std::size_t i = 0; i < drawables.size(); ++i) {
            if (drawables[i] && positions[i]) {
                sf::RectangleShape shape(sf::Vector2f(50.0f, 50.0f));
                shape.setPosition(positions[i]->x, positions[i]->y);
                shape.setFillColor(sf::Color::Green);
                _window.draw(shape);
            }
        }
    }

  private:
    sf::RenderWindow& _window;
};
```

The draw system is responsible for drawing the entities with a Position and Drawable component.

You can see that the update method takes a Registry object as a parameter. The Registry object is a class that manages the entities and their components.

It then gets the Position and Drawable components from the registry and iterates over them to draw the entities.