---
title: Build System
parent: Technology choices
grand_parent: Developper
nav_order: 2
---
# Technology choices - Build System

When evaluating build systems for our project, we considered several options, including CMake, Meson. After careful evaluation, we decided to use CMake as our build system for the following reasons:

## Table of Contents

   * [Build System](#build-system)
   * [1. Cross-Platform Support](#1-cross-platform-support)
   * [2. IDE Integration](#2-ide-integration)
   * [3. Flexibility](#3-flexibility)
   * [4. Community Support](#4-community-support)

### 1. Cross-Platform Support

- Designed to work on **Windows**, **macOS**, and **Linux**.  
- Ensures the project can be built and run on a wide range of systems without major modifications.  

### 2. IDE Integration

- Integrates seamlessly with popular IDEs like **Visual Studio**, **CLion**, and **Xcode**.  
- Provides a **consistent development experience** for team members using different tools.  

### 3. Flexibility

- Offers a **high degree of flexibility** for configuring build options, dependencies, and target platforms.  
- Enables customization of the build process to meet **specific project requirements**.  

### 4. Community Support

- Backed by a **large and active community**.  
- Provides **extensive documentation**, **tutorials**, and **support resources**.  
- Simplifies troubleshooting and promotes adoption of **best practices**.  
