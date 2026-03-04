# Simulation Engine

A deterministic 2D simulation engine written in C++ using SFML.

The purpose of this project is to study and implement core simulation architecture including deterministic update pipelines, system based design, and reproducible simulation behavior. The project is not intended to be a game engine. Instead it is an experimental environment for understanding how simulation systems evolve and interact over time.

## Current Features

Fixed timestep simulation loop with accumulator based update control

Custom Vec2 math system supporting vector arithmetic and normalization

Entity based simulation objects containing position, velocity, acceleration, mass, radius, energy, and lifecycle state

World manager responsible for entity ownership, ordered system execution, physics integration, and cleanup

System architecture where simulation rules operate over populations of entities

Spawn request pipeline where input generates deterministic spawn events processed during simulation updates

Constant force simulation with semi implicit Euler integration and linear damping

Boundary collision resolution with velocity reflection

Deterministic simulation timeline using a discrete frame index

Simulation time authority supporting pause, slow motion, and single step debugging

Input recording and replay infrastructure allowing deterministic re simulation of recorded runs

Basic rendering pipeline using SFML for visualizing simulation state

## Goals

Build a stable deterministic simulation framework that can support long running experiments

Explore emergent behavior and artificial agent systems within controlled simulation environments

Develop scalable system oriented architecture suitable for complex simulation rules

Study deterministic replay techniques used in physics engines and networked simulations

Use the project as a platform for learning simulation design, engine architecture, and reproducible systems

## Tech Stack

C++20

SFML 3

Visual Studio

Git and GitHub

## Status

Early stage development. The current focus is on deterministic simulation architecture, replay foundations, and system level design before expanding into more advanced simulation features.

## Author

Mallikarjun Shankar