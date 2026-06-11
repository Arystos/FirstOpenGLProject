# OpenGL 3D Renderer (from scratch)

A small real-time 3D renderer written from scratch in **C++** and **OpenGL** — built to learn the
graphics pipeline end to end, without leaning on a game engine.

<!-- TODO: add a screenshot or GIF of the rendered scene here -->

## What it does
- Renders 3D meshes with **indexed geometry**
- Free-fly **camera** (WASD + mouse look)
- **Texture** loading via `stb_image` and sampling in-shader
- **Phong lighting** (ambient / diffuse / specular) with a separate light source object
- Thin, reusable abstractions over raw OpenGL handles: `VAO`, `VBO`, `EBO`, `Texture`, `shaderClass`
- GLSL shaders: `default.vert` / `default.frag` (lit objects) and `light.vert` / `light.frag` (light source)

## Tech
C++ · OpenGL · GLAD · GLFW · GLM · stb_image

## Build
Open `OpenGLProject.sln` in **Visual Studio 2022** and build for **x64**.
Third-party dependencies are vendored under `Libraries/`, so no extra setup is required.

## Why it's here
This was my first deep dive into low-level real-time rendering — figuring out how vertices, buffers,
shaders and the transformation pipeline actually fit together *beneath* a game engine.
