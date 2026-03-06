# Deterministic Simulation Sandbox

A deterministic 2D simulation sandbox built from scratch in C++ using SFML 3.

This project focuses on building a reproducible simulation environment with a clean architecture rather than creating a traditional game engine. The system is designed to explore deterministic update pipelines, replayable simulations, and controlled experimentation within a physics-based environment.

The long term objective is to evolve the engine into a platform for studying artificial agents, emergent behavior, and complex system simulations.

---

## Core Characteristics

Deterministic execution is the central design principle. Given the same initial state, ordered inputs, and timestep, the simulation must always produce identical results.

The architecture emphasizes:

* explicit ownership of simulation state
* strict authority boundaries
* replayable command streams
* reproducible simulation runs
* architecture-first development

All simulation state changes occur exclusively inside the `World.update()` step.

---

## Architecture Overview

Execution hierarchy:

```
main()
  → App
      → OperatorController
          → World
              → Systems
                  → Entities
                      → Vec2
```

### Responsibilities

**App**

Handles the runtime environment, window management, and rendering.

**OperatorController**

Interprets user input and converts it into simulation commands.

**World**

Owns and advances the entire simulation state.
World is the sole authority for all simulation changes.

**Systems**

Operate on entity populations and apply simulation rules.

**Entities**

State containers representing physical objects in the simulation.

---

## Implemented Systems

### ForceSystem

Applies constant forces to entities.

### SpawnSystem

Processes queued spawn requests and creates entities through the world pipeline.

---

## Deterministic Replay System

The engine records operator commands and can replay them deterministically.

Recorded commands include:

* entity spawning
* pause / resume
* single frame stepping
* time scale adjustments
* simulation reset events

During replay the engine re-injects recorded commands at the exact simulation frame they originally occurred.

A world state hash is generated every frame and compared during replay to detect divergence.

---

## Simulation Features

Current capabilities include:

* deterministic fixed timestep simulation
* entity spawning through a queued pipeline
* semi-implicit Euler integration
* linear damping
* boundary collision and reflection
* deterministic replay and verification
* state hash validation
* simulation pause and single-step debugging
* adjustable simulation time scale
* simulation reset and replay controls
* real-time debug overlay

The debug overlay displays:

* frame number
* entity count
* current time scale
* pause state
* replay state

---

## Operator Controls

| Input       | Action                    |
| ----------- | ------------------------- |
| Left Click  | Spawn entity              |
| Space       | Pause / resume simulation |
| Right Arrow | Advance one frame         |
| Up Arrow    | Increase time scale       |
| Down Arrow  | Decrease time scale       |
| R           | Reset time scale          |
| C           | Clear simulation          |
| T           | Start replay              |

---

## Technology Stack

Language: C++20
Graphics Library: SFML 3
IDE: Visual Studio
Platform: Windows

Compiler settings:

* x64
* /W4
* /WX (warnings treated as errors)

---

## Project Goals

The engine is being developed to study:

* deterministic simulation architecture
* reproducible simulation environments
* simulation debugging tools
* command stream replay systems
* emergent behavior foundations
* artificial agent experimentation

The project prioritizes architectural clarity and determinism over rapid feature development.

---

## Development Philosophy

The engine follows strict rules:

* all simulation changes occur inside `World.update()`
* systems do not create or destroy entities directly
* input records operator intent rather than mutating state
* randomness affecting simulation outcomes must be deterministic or recorded

These constraints ensure simulations remain reproducible and debuggable.

---

## Current Status

The project currently functions as a deterministic simulation sandbox capable of recording and replaying operator actions while validating simulation consistency.

Future development will expand the sandbox with features such as:

* simulation snapshots and rollback
* experiment branching
* entity inspection tools
* scenario loading
* artificial agent simulation systems

---

## Author

Mallikarjun Shankar
