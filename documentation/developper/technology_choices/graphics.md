# Technology Choices - Graphics

Table of Contents
=================

   * [Graphics](#graphics)
   * [1. **SFML (Chosen Option)**](#1-sfml-chosen-option)
   * [2. **OpenGL**](#2-opengl)

### 1. **SFML (Chosen Option)**

- **Cross-Platform**:  
  - SFML is fully cross-platform, and integration with **vcpkg** makes it easy to manage across **Windows**, **Linux**, and **macOS**.  

- **Ease of Use vs. Feature-Richness**:  
  - Provides a **perfect balance** between being easy to use and offering rich functionality.  
  - Can handle **almost anything** needed for 2D game development with minimal effort.  

- **Optimized for 2D**:  
  - Designed with **2D graphics** in mind, making it a better fit for our project's requirements.  
  - Simplifies implementation for a **2D aesthetic**, compared to lower-level libraries.  

- **Team Experience**:  
  - All team members have prior experience with SFML, which reduces the learning curve and accelerates development.  

- **Audio support**:  
  - Offers built-in support for **audio**, which is essential for creating a good atmosphere in games.

---

### 2. **OpenGL**

- **Strengths**:  
  - Offers **fine-grained control** over rendering and pixel-level manipulation on the GPU.  
  - A powerful tool for advanced and complex graphical projects requiring deep customization.  
  - Ideal for **3D rendering**, where more control over shaders, buffers, and custom pipelines is needed.  

- **Why Not Chosen**:  
  - Introduces a **higher level of complexity**, which was unnecessary for our project.
  - Requires significant effort and time, which we could not afford given our goal of creating a simple 2D game.  
  - Only **two team members** had prior experience with OpenGL, potentially slowing down the development process.  
  - **More boilerplate code** is required to set up a simple rendering pipeline, as compared to the more abstracted and user-friendly API of SFML.  
  - **Lack of integrated higher-level functionality**: OpenGL does not offer built-in support for tasks like window creation, events handling, and sound, which are needed for a game. These would require additional libraries or a lot of extra effort to implement on top of OpenGL.  






