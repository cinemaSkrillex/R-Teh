# Technology choices - Networking

When evaluating networking libraries for our project, we considered several options:
- Boost.Asio
- Enet
- OS-specific network API (eg. Unix
BSD Sockets on Linux for example, or Windows Sockets on Windows).

After careful evaluation, we decided to use Boost.Asio as our networking library for the following reasons:

## Table of Contents

   * [Networking](#networking)
   * [1. Performance and Scalability](#1-performance-and-scalability)
   * [2. Cross-Platform Support](#2-cross-platform-support)
   * [3. Asynchronous I/O](#3-asynchronous-io)
   * [4. Community Support](#4-community-support)

### 1. Performance and Scalability

- Boost.Asio is known for its high performance and scalability, making it suitable for real-time applications like games. It provides efficient I/O operations and supports asynchronous programming models, which are essential for handling multiple connections and events concurrently.

### 2. Cross-Platform Support

- Boost.Asio is designed to work across different platforms, including Windows, macOS, and Linux. This ensures that our networking code can be ported easily to various systems without major modifications, simplifying the development and deployment process.

### 3. Asynchronous I/O

- Boost.Asio offers robust support for asynchronous I/O operations, allowing our application to perform non-blocking reads and writes efficiently. This feature is crucial for handling network communication without blocking the main thread, ensuring smooth gameplay and responsiveness.

### 4. Community Support

- Boost.Asio has a dedicated community of developers who actively contribute to its development and provide support through forums, documentation, and tutorials. This extensive community support makes it easier for our team to learn and leverage the library effectively, reducing the learning curve and accelerating development.

By choosing Boost.Asio as our networking library, we aim to build a robust and efficient networking layer for our game that can handle the demands of real-time multiplayer gameplay effectively. The performance, cross-platform compatibility, asynchronous I/O capabilities, and community support of Boost.Asio align with our project requirements and development goals, making it the ideal choice for our networking needs.