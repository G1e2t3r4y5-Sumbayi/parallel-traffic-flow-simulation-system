# parallel-traffic-flow-simulation-system

This project contains a simple hybrid traffic simulation that combines shared-memory parallelism (OpenMP) with a simulated distributed model for intersections. It was created for *COMP 412 – Parallel and Distributed Systems* to illustrate how road segments and intersections can be processed concurrently.
## Features

- *Parallel road updates* using #pragma omp parallel for to simulate vehicle movement under traffic light control.
- *Intersection handoff* between road segments with basic locking (atomic/critical sections) for safe shared-state updates.
- *Configurable simulation parameters* such as number of roads, intersections, maximum vehicles, and time steps (see traffic_simulation.c).
