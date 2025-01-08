---
title: Create a system Documentation 🧩
parent: Systems (ECS)
grand_parent: ECS
nav_order: 2
---

# Create a system Documentation 🧩

This is a guide on how to create a documentation concerning the systems of the ECS.

If you have created a new system, you should document it using our template.

## Template

```markdown
---
title: System Name
parent: ECS
grand_parent: Developper
nav_order: 1
---

# System Name

## Description

A brief description of the system.

## Signature

CODE BLOCK : 
void SystemName::update(float dt);


## Parameters

- `dt`: The time elapsed since the last frame.

## Example


void MovementSystem::update(float dt) {
    for (auto entity : entities) {
        auto& position = registry.get<PositionComponent>(entity);
        auto& velocity = registry.get<VelocityComponent>(entity);

        position.x += velocity.x * dt;
        position.y += velocity.y * dt;
    }
}


## Usage

How to use the system in the game.

## Notes

Any additional notes about the system.
```

Thanks for contributing! 🎉
