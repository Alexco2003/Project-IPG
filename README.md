# 🚜 Farm Rush

**Farm Rush** is a 3D endless runner game developed in **C++ and OpenGL**.

Heavily inspired by the classic **Subway Surfers**, this project reimagines the genre in a rural, nocturnal setting. You take control of a tractor charging through an endless farm, dodging hay bales, scarecrows, and horses, all while navigating a world that dynamically curves beneath your wheels.

> **Project Context:** This game was developed for the [Introduction to Computer Graphics](https://ocw.cs.pub.ro/courses/ipg) course, fulfilling the requirements of the [2025 Project Assignment](https://ocw.cs.pub.ro/courses/ipg/tema/2025).

> 👉 [Watch Farm Rush - Gameplay Demo](https://youtu.be/OFpaMOxxv-g)
>
> [![Watch on YouTube](https://img.shields.io/badge/Watch_Demo-YouTube-FF0000?style=for-the-badge&logo=youtube&logoColor=white)](https://youtu.be/OFpaMOxxv-g)

## 🎮 Gameplay Features

* **Procedural Level Generation:** Utilizes a deterministic pseudo-random hashing algorithm to spawn environmental assets (trees, windmills) and a randomized pooling system for obstacles, ensuring a unique layout for every run.
* **Curved World Effect:** A custom vertex shader bends the geometry of the world, recreating the iconic "horizon" effect found in *Subway Surfers*.
* **Lighting & Atmosphere:** A rich night-time environment featuring toggleable tractor headlights (Low/High Beams), illuminated street lamps, and glowing windmills.
* **Shadow Mapping:** Implemented custom shadow mapping using Framebuffers and PCF, allowing obstacles to cast dynamic shadows on the ground based on the tractor's high beams.
* **Continuous Animations:** Features smooth animations including interpolated steering, velocity-based wheel rotation, and dynamic chassis rumble (engine vibration) that intensifies with movement.
* **Collision System:** Utilizes a radial distance-based detection algorithm to calculate precise impacts between the player and obstacles.
* **Progression Mechanics:** Distance-based scoring system with dynamic difficulty scaling - the tractor's forward speed automatically increases as the score gets higher.

## 🕹️ Controls

| Key | Action |
| :---: | :--- |
| **W** / **S** | Accelerate / Brake |
| **A** / **D** | Steer Left / Right |
| **F** | Toggle Headlights (Low Beams) |
| **G** | Toggle High Beams |
| **R** | Restart Game (After Game Over) |
| **ESC** | Quit Game |

## 📥 Download & Installation

You can download the latest playable build from the [Releases](../../releases) tab of this repository.

1.  Go to the **Releases** section on the right sidebar of this page.
2.  Download the latest **`.zip`** archive.
3.  Extract the archive to a folder on your computer.
4.  Run **`Farm Run.exe`** to start the game.

## ⚙️ Development Setup

If you want to build the project from source or modify the code, you need to configure the development environment first.

Please refer to the internal documentation found in the framework folder or check the official setup tutorial:

* 📖 **Framework Documentation:** [Read the Framework README](./gfx-framework-ipg-master/README.md)
* 🎓 **Step-by-Step Tutorial:** [OCW Setup Guide](https://ocw.cs.pub.ro/courses/ipg/setup-framework)

## 🛠️ Built With

* **C++**
* **OpenGL**
* **Custom Shaders (GLSL)**
* **GFXFramework**

---
*Drive safe and watch out for the horses!* 🐎
