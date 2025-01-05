---
title: Package Manager
parent: Technology choices
grand_parent: Developper
nav_order: 1
---
# Technology choices - Package Manager

When evaluating package managers for C++ projects, we considered both vcpkg and Conan. Ultimately, we chose vcpkg for several reasons, despite not conducting an extensive test of Conan. Below is a summary of our rationale:

Table of Contents
=================

   * [Technology choices - Package Manager](#technology-choices---package-manager)
   * [Table of Contents](#table-of-contents)
      * [1. Installation Requirements](#1-installation-requirements)
      * [2. Maintained or Backed by a Major Organization](#2-maintained-or-backed-by-a-major-organization)
      * [3. Ecosystem Integration](#3-ecosystem-integration)
      * [4. Simplicity and Learning Curve](#4-simplicity-and-learning-curve)
      * [5. Security Considerations](#5-security-considerations)
      * [6. Our own friends](#6-our-own-friends)

### 1. Installation Requirements

- **Conan**:  
  - Requires **Python** for installation, introducing additional dependencies and potential challenges.  
  - Relies on **PIP**, which can pose a security risk if not properly managed.  

- **vcpkg**:  
  - Only requires a simple **git clone**, making it lightweight and easy to set up.  
  - Simplicity was a key factor in our decision.  

### 2. Maintained or Backed by a Major Organization

- **vcpkg**:  
  - Actively maintained by **Microsoft**, ensuring strong integration with the Windows ecosystem.  
  - Provides a higher level of **trust and reliability**.  

- **Conan**:  
  - Community-driven and widely respected, but lacks direct backing by a major organization like Microsoft.  

### 3. Ecosystem Integration

- **vcpkg**:  
  - Offers **seamless integration** with Microsoft tools like **Visual Studio** and **MSBuild**.  
  - Highly attractive for our predominantly **Windows-based development environment**.  

- **Conan**:  
  - Provides broad **cross-platform support** but requires more configuration, especially for projects using Microsoft technologies.  

### 4. Simplicity and Learning Curve

- **vcpkg**:  
  - **Straightforward to use**, with minimal configuration required for C++ projects.  
  - Developers can quickly get started without the need for profiles or extensive setup.  
  - **Ideal for beginners** or those looking for a simple package manager, because vcpkg uses only CMake for configuration., Conan uses Python.

- **Conan**:  
  - While more **flexible**, it has a **steeper learning curve** due to features like profiles and Python-based configurations.  

### 5. Security Considerations

- **Conan**:  
  - The reliance on **PIP** and **Python** introduces potential vectors for dependency-related vulnerabilities.  

- **vcpkg**:  
  - Simpler installation process reduces potential security risks.  

### 6. Influence of Our Peers

- Our friends developing **R-Type** are using **vcpkg** and shared that it is the **easiest to use**.  
- We trust their experience and know they are leveraging it wisely.  


