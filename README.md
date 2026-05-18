# 2D Physics Engine

A custom-built 2D Physics Engine written in C++ using SDL3, Vulkan, and GLM, designed to simulate real-time rigid body physics with a fully interactive editor and rendering pipeline.

This project focuses on building a complete physics engine from scratch, including collision detection, impulse-based resolution, rigid body dynamics, Vulkan rendering, and an integrated editor workflow.

The engine is being developed as a long-term systems programming and graphics engineering project with a modular architecture separating physics, rendering, editor systems, and platform abstraction layers. :contentReference[oaicite:0]{index=0}

---

# 🚧 Project Status

### Current Progress
- Core engine modules completed
- Physics architecture designed
- Vulkan surface creation completed
- SDL3 integration completed
- Project scaffolding and module structure established
- Core mathematics system implemented/planned
- Physics simulation pipeline under active development

### In Progress
- Vulkan renderer pipeline
- SAT collision system
- Impulse solver integration
- Interactive editor modes

---

## Features

### Physics Engine
- Rigid body dynamics
- Impulse-based collision resolution
- Semi-implicit Euler integration
- Circle and convex polygon support
- SAT collision detection
- AABB broad-phase collision system
- Friction and restitution handling
- Gravity simulation
- Angular dynamics

### Rendering System
- Vulkan-based renderer
- GPU-accelerated rendering pipeline
- Dynamic geometry batching
- Debug rendering overlays
- SDL3 Vulkan surface integration
- Real-time simulation rendering

### Editor System
- Object creation mode
- Physics properties editor
- Simulation mode
- Polygon sculpting tools
- Interactive rigid body manipulation
- Debug visualization tools

---

## Tech Stack

### Core Technologies
- C++20
- SDL3
- Vulkan
- GLM

### Build & Dependency Management
- CMake
- vcpkg

---

## Purpose of the Project

This project was built to deeply understand:
- Physics engine architecture
- Rigid body simulation
- Collision detection algorithms
- Graphics programming
- Vulkan rendering pipelines
- GPU resource management
- Low-level systems programming
- Real-time simulation workflows
- Engine architecture design

---

## Engine Architecture

The engine is split into multiple independent systems:

```text
Platform Layer
        ↓
Renderer Layer (Vulkan)
        ↓
Physics Core
        ↓
Editor & Application Layer
```

The modular architecture allows:
- renderer-independent physics,
- scalable editor systems,
- and clean subsystem communication. :contentReference[oaicite:1]{index=1}

---

## Core Engine Modules

### Physics Core
- `RigidBody`
- `Shape`
- `CollisionDetector`
- `CollisionResolver`
- `PhysicsWorld`
- `AABB`
- `Vec2`

### Rendering Layer
- `VulkanContext`
- `Pipeline`
- `GeometryBatch`
- `DebugDraw`

### Editor System
- `CreateMode`
- `PropertiesMode`
- `SimulateMode`

### Platform Layer
- SDL3 window management
- Input handling
- Fixed timestep loop

:contentReference[oaicite:2]{index=2}

---

## Physics Features

### Collision Detection
- AABB broad phase
- SAT narrow phase
- Circle vs Circle collision
- Polygon vs Polygon collision
- Polygon vs Circle collision

### Collision Resolution
- Impulse solver
- Friction simulation
- Angular impulse handling
- Positional correction (Baumgarte stabilization)

:contentReference[oaicite:3]{index=3}

---

## Rendering Pipeline

The renderer uses Vulkan for explicit GPU control and low-level rendering management.

### Current Vulkan Workflow
- Vulkan instance creation
- SDL3 Vulkan surface creation
- Physical device selection
- Swapchain management
- Render pass creation
- Dynamic vertex batching

The rendering system is designed specifically for:
- real-time physics visualization,
- debug overlays,
- and efficient 2D rendering workflows. :contentReference[oaicite:4]{index=4}

---

## Editor Modes

### Create Mode
- Circle tool
- Rectangle tool
- Polygon sculpting
- Vertex editing
- Object selection

### Properties Mode
- Mass editing
- Friction/restitution controls
- Velocity editing
- Static body configuration

### Simulation Mode
- Play/pause controls
- Gravity manipulation
- Debug overlays
- Step-by-step simulation

:contentReference[oaicite:5]{index=5}

---

## Current Development Roadmap

### Completed
- Project setup
- SDL3 integration
- Vulkan surface creation
- Engine architecture planning
- Core module design

### Upcoming
- Vulkan rendering pipeline completion
- SAT collision system
- Impulse solver
- Editor interaction tools
- Simulation controls
- Debug rendering

### Future Goals
- Joint constraints
- Soft body simulation
- Broad-phase BVH optimization
- Continuous collision detection
- Scene serialization
- Texture mapping
- Multithreaded rendering

:contentReference[oaicite:6]{index=6}

---

## Installation

Clone the repository:

```bash
git clone https://github.com/your-username/2D-Physics-Engine.git
```

Move into the project directory:

```bash
cd 2D-Physics-Engine
```

Install dependencies using vcpkg:

```bash
vcpkg install sdl3 glm
```

Configure the project:

```bash
cmake -B build
```

Build the project:

```bash
cmake --build build
```

Run the executable:

```bash
./build/PhysicsEngine2D
```

---

## Project Structure

```text
PhysicsEngine2D/
│
├── src/
│   ├── core/
│   ├── renderer/
│   ├── editor/
│   ├── platform/
│   └── main.cpp
│
├── shaders/
├── docs/
├── CMakeLists.txt
├── vcpkg.json
└── README.md
```

---

## Learning Outcomes

This project is helping me understand:
- Advanced C++ systems programming
- Vulkan graphics programming
- Real-time physics simulation
- Collision detection mathematics
- GPU rendering architecture
- Engine modularity
- Memory management
- Physics solver design
- Interactive editor workflows

---

## Documentation

Detailed engine architecture and build documentation is included in the project documentation files covering:
- system architecture,
- physics algorithms,
- Vulkan rendering pipeline,
- collision systems,
- and editor workflows. :contentReference[oaicite:7]{index=7}

---

## License

This project is open-source and available under the MIT License.
