---
title: Technology choices - Networking
parent: Technology choices
grand_parent: Developper
nav_order: 5
---
# Technology choices - Networking

When evaluating networking libraries for our project, we considered several options:
- Boost.Asio
- Enet
- OS-specific network API (eg. Unix
BSD Sockets on Linux for example, or Windows Sockets on Windows).

After careful evaluation, we decided to use Boost.Asio as our networking library for the following reasons:

## Table of Contents

   * [Networking](#networking)
   * [Why Boost.Asio?](#why-boostasio)
   * [1. Performance and Scalability](#1-performance-and-scalability)
   * [2. Cross-Platform Support](#2-cross-platform-support)
   * [3. Asynchronous I/O](#3-asynchronous-io)
   * [4. Community Support](#4-community-support)
   * [5. Documentation](#5-documentation)



### Why Boost.Asio? 

### 1. Performance and Scalability

- Known for **high performance and scalability**, ideal for real-time applications like games.  
- Provides **efficient I/O operations** for handling high loads.  
- Supports **asynchronous programming models**, enabling concurrent management of multiple connections and events.  

### 2. Cross-Platform Support

- Works seamlessly on **Windows**, **macOS**, and **Linux**.  
- Enables easy portability of networking code across platforms.  
- Requires **minimal modifications** during development and deployment.  
- Simplifies cross-platform development and reduces overhead.  

### 3. Asynchronous I/O

- Provides robust support for **asynchronous I/O operations**.  
- Enables **non-blocking reads and writes** for efficient communication.  
- Prevents **blocking the main thread**, ensuring smooth gameplay and responsiveness.  

### 4. Community Support

- Backed by a **dedicated community of developers**.  
- Offers extensive resources such as **forums**, **documentation**, and **tutorials**.  
- Eases the learning curve, helping the team adopt the library quickly.  
- Accelerates development through accessible community support.  

- By choosing Boost.Asio, we aim to build a **robust and efficient networking layer** capable of handling the demands of **real-time multiplayer gameplay**. Its performance, cross-platform compatibility, asynchronous I/O capabilities, and strong community support make it the **ideal choice** for our project.  

### 5. Documentation

- Features **comprehensive and well-maintained documentation**.  
- Facilitates a **quick start** with the library.  
- Simplifies **troubleshooting** during development.  
- Enhances productivity by providing **clear guidance** for implementation.  


### Other Options Considered

- **Enet**:  
  - A lightweight networking library that offers **simplicity and ease of use**.  
  - Provides an implementation for **UDP reliable packets**, a common use case for games.  

  **Why we chose Boost.Asio over Enet**:  
  - Boost.Asio has a **broader feature set**, supporting both **TCP** and **UDP**.  
  - Using a single library for both protocols simplifies development and ensures **consistency** in the networking codebase.  

- **OS-specific Network API**:  
  - Offers **low-level control and optimization** for network operations.  
  - Can introduce platform dependencies and complexities, impacting **cross-platform compatibility** and **portability**.  

  **Why we chose Boost.Asio over OS-specific APIs**:  
  - Boost.Asio ensures **portability** across platforms like **Windows** and **Linux**.  
  - Provides a consistent and easy-to-use API, reducing development complexity while maintaining cross-platform support.  
