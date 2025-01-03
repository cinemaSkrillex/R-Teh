# Technology choices - Build System

When evaluating build systems for our project, we considered several options, including CMake, Meson, and Bazel. After careful evaluation, we decided to use CMake as our build system for the following reasons:

## Table of Contents

   * [Build System](#build-system)
   * [1. Cross-Platform Support](#1-cross-platform-support)
   * [2. IDE Integration](#2-ide-integration)
   * [3. Flexibility](#3-flexibility)
   * [4. Community Support](#4-community-support)

### 1. Cross-Platform Support

- CMake is designed to work across different platforms, including Windows, macOS, and Linux. This ensures that our project can be built and run on a wide range of systems without major modifications.

### 2. IDE Integration

- CMake integrates well with popular IDEs like Visual Studio, CLion, and Xcode, providing a seamless development experience for our team members who use different tools.

### 3. Flexibility

- CMake offers a high degree of flexibility in configuring build options, dependencies, and target platforms. This allows us to customize the build process to suit our specific requirements.

### 4. Community Support

- CMake has a large and active community that provides extensive documentation, tutorials, and support resources. This makes it easier for our team to troubleshoot issues and leverage best practices.