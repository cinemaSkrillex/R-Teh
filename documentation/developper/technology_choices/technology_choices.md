# Technology choices

## Package Manager

When evaluating package managers for C++ projects, we considered both vcpkg and Conan. Ultimately, we chose vcpkg for several reasons, despite not conducting an extensive test of Conan. Below is a summary of our rationale:

### 1. Installation Requirements

- Conan: Requires Python for installation, which may introduce additional dependencies and potential challenges. It also relies on PIP, which can pose a security risk if not properly managed.

- vcpkg: Only requires a simple git clone, making it more lightweight and easier to set up. This simplicity was a key factor in our decision.

### 2. Maintained or Backed by a Major Organization

- vcpkg is actively maintained by Microsoft, ensuring strong integration with the Windows ecosystem and providing a level of trust and reliability.

- While Conan is community-driven and widely respected, the direct backing of Microsoft gave vcpkg an edge for our use case.

### 3. Ecosystem Integration

- vcpkg: Seamless integration with Microsoft tools like Visual Studio and MSBuild. This made it highly attractive for our predominantly Windows-based development environment.

- Conan: Offers broad cross-platform support but requires more configuration, particularly for projects relying on Microsoft technologies.

### 4. Simplicity and Learning Curve

- vcpkg: Straightforward to use, with minimal configuration required for C++ projects. Developers can quickly get started without the need for profiles or extensive setup.

- Conan: While more flexible, it has a steeper learning curve due to features like profiles and Python-based configurations.

### 5. Security Considerations

- The reliance of Conan on PIP and Python introduces potential vectors for dependency-related vulnerabilities. vcpkg’s simpler installation process reduces this risk.

### 6. Our own friends

Our friends making rtype are using vcpkg, and told us it's the easiest to use. We trust them, and we know that they are using it wisely

## Build System

For the build system, we chose CMake due to its widespread adoption, flexibility, and compatibility with various platforms and IDEs. Here are some key reasons behind our decision:

### 1. Cross-Platform Support

- CMake is designed to work across different platforms, including Windows, macOS, and Linux. This ensures that our project can be built and run on a wide range of systems without major modifications.

### 2. IDE Integration

- CMake integrates well with popular IDEs like Visual Studio, CLion, and Xcode, providing a seamless development experience for our team members who use different tools.

### 3. Flexibility

- CMake offers a high degree of flexibility in configuring build options, dependencies, and target platforms. This allows us to customize the build process to suit our specific requirements.

### 4. Community Support

- CMake has a large and active community that provides extensive documentation, tutorials, and support resources. This makes it easier for our team to troubleshoot issues and leverage best practices.