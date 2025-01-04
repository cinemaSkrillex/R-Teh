---
title: Create a component Documentation
parent: ECS
grand_parent: Developper
nav_order: 3
---

# Create a component Documentation 🧩

This is a guide on how to create a documentation concerning the components of the ECS.

If you have created a new component, you should document it using our template.

## Template

```markdown
---
title: Component Name
parent: ECS
grand_parent: Developper
nav_order: 1
---

# Component Name

## Description

A brief description of the component.

## Signature

CODE BLOCK: 

struct ComponentName {
    // Data members
};

## Example

CODE BLOCK:

struct PositionComponent {
    float x;
    float y;
};


## Usage

How to use the component in the game.

## Notes

Any additional notes about the component.
```

Thanks for contributing! 🎉