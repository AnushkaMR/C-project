# libtiny3d — Lightweight 3D Software Renderer in C

**libtiny3d** is a lightweight 3D rendering library built from scratch in C. It implements a basic 3D graphics pipeline, including geometric transformations, perspective projection, depth buffering, wireframe rendering, and Lambertian lighting.

The project demonstrates the fundamentals of 3D computer graphics without relying on a dedicated graphics engine. It also includes procedural geometry generators, rendering tests, and animation tools for experimenting with 3D scenes.

## ✨ Features

- **3D Mathematics** — Vector operations, matrix transformations, and camera calculations.
- **Perspective Projection** — Projects 3D coordinates onto a 2D canvas using a perspective frustum.
- **Software Rendering** — Renders 3D geometry directly onto a pixel-based canvas.
- **Wireframe Rendering** — Visualizes meshes through their edges and connections.
- **Lighting System** — Supports multi-directional Lambertian lighting for basic surface shading.
- **Depth Buffering** — Uses a Z-buffer to manage pixel depth.
- **Procedural Mesh Generation** — Creates 3D objects such as cubes, cylinders, pyramids, and soccer balls.
- **Bezier Animation** — Provides cubic Bézier curves for smooth animation paths.
- **Automated Testing** — Includes mathematical unit tests and visual rendering pipeline tests.
- **Animation Export** — Generates animated GIFs from rendered image sequences using ImageMagick.

## 📂 Project Structure

```text
libtiny3d/
├── include/       # Public headers and API declarations
├── src/           # Core rendering implementation
├── models/        # Procedural 3D geometry generators
├── demo/          # Example application
├── tests/         # Unit tests and visual rendering tests
├── build/         # Compiled executables and generated output
└── Makefile       # Build, run, test, and cleanup commands
```

## 🛠️ Technologies

- **Language:** C
- **Build System:** GNU Make
- **Graphics:** Custom software rendering pipeline
- **Testing:** Mathematical and visual rendering tests
- **Animation:** ImageMagick

## 🚀 Getting Started

### Build the Project

Compile the demo application and test programs:

```bash
make
```

### Run the Demo

```bash
make run_main
```

This runs the main demo and generates rendered frames in the configured output directory.

### Run Tests

Execute the mathematical unit tests:

```bash
make run_math
```

Run the rendering pipeline tests:

```bash
make run_pipeline
```

### Generate Animations

Create animated GIFs from the generated frames:

```bash
make gif_main
make gif_soccer
```

To generate all supported animations:

```bash
make all_gifs
```

**Requirement:** ImageMagick must be installed and its `magick` command available in the terminal.

### Clean Generated Files

```bash
make clean
make clean_gif
```

## 🧩 Core Modules

| Module | Responsibility |
|---|---|
| `canvas.h` | Canvas creation, pixel operations, depth buffering, and image output |
| `math3d.h` | 3D vectors, matrix operations, transformations, and camera mathematics |
| `renderer.h` | Mesh projection, viewport clipping, and geometry rendering |
| `lighting.h` | Lambertian lighting and multi-light brightness calculations |
| `animation.h` | Bézier curves for animation paths |

## 🎮 Example Use Case

The library can generate a 3D soccer ball, apply rotation and scaling transformations, configure a camera and perspective projection, calculate lighting, and render the resulting scene onto a canvas.

Geometry is currently generated programmatically through C functions rather than loaded from external 3D model files.

## 🎯 Project Goals

The primary goal of `libtiny3d` is to explore the foundations of computer graphics by implementing a compact rendering pipeline in C. It provides a practical environment for learning about 3D mathematics, projection, mesh representation, lighting, depth management, and frame-based animation.

