---
title: Repository structure
parent: Developper
has_children: true
nav_order: 1
---

# Repository structure deep dive

## Overview

This document describes the structure of the repository and the purpose of each directory.

## Diagram

You can find the diagram of the repository structure [here](repository_structure_diagram.md).

## Repository structure

You can see a tree representation of the repository structure inside the "repository_structure_tree.txt" file.

## Base directories

`.github` - Contains GitHub Actions workflows

`documentation` - Contains the documentation files.

`assets` - Contains the images and other assets used in the game.

`Scripts` - Contains the Scripts used to automate tasks.

`RtypeServer` - Contains the server-side code.

`RtypeGame` - Contains the game client-side code.

`Network` - Contains the Network protocol definition (UDP and TCP server/client).

`GameEngine` - Contains the game engine code.

## Advanced directories

`GameEngine/src/Core` - Contains the core game engine code. (e.g. Clock, View, Window, Images, Sounds, etc.)

`GameEngine/src/ECS` - Contains the Entity-Component-System code.

`GameEngine/src/ECS/Entities` - Contains the entities code (spawn, kill entities, etc.)

`GameEngine/src/ECS/Registry` - Contains the registry code (add, remove components, etc.)

`GameEngine/src/ECS/SparseArray` - Contains the sparse array code for the ECS.

`GameEngine/src/ECS/Systems` - Contains the systems code, for example, the MovementSystem, CollisionSystem, etc.

`GameEngine/src/Scripting` - Contains the scripting code : Lua via sol2
