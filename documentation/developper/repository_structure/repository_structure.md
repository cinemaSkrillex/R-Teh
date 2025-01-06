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

`scripts` - Contains the scripts used to automate tasks.

`rtype_server` - Contains the server-side code.

`rtype_game` - Contains the game client-side code.

`network` - Contains the network protocol definition (UDP and TCP server/client).

`game_engine` - Contains the game engine code.

## Advanced directories

`game_engine/src/Core` - Contains the core game engine code. (e.g. Clock, View, Window, etc.)

`game_engine/src/ECS` - Contains the Entity-Component-System code.

`game_engine/src/ECS/Entities` - Contains the entities code (spawn, kill entities, etc.)

`game_engine/src/ECS/Registry` - Contains the registry code (add, remove components, etc.)

`game_engine/src/ECS/SparseArray` - Contains the sparse array code for the ECS.

`game_engine/src/ECS/Systems` - Contains the systems code, for example, the MovementSystem, CollisionSystem, etc.

`game_engine/src/Media/Audio` - Contains the media code (e.g. Image, Sound, etc.)

`game_engine/src/Media/Graphic` - Contains the video code (e.g. Sprite, Animation, Text, etc.)

`game_engine/src/Scripting` - Contains the scripting code : Lua via sol2