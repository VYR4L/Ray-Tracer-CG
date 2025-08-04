# Ray-Tracer-CG

## Overview

This project is an assignment for the Computer Graphics class. It implements and experiments with the ray tracing techniques described in the three books of the "Ray Tracing In One Weekend" series by Peter Shirley.

## Features

- Builds and runs the source codes from all three books in the series.
- Will include a custom scene with:
  - 5 objects of different shapes and sizes
  - 5 different viewports
  - 3 original materials (to be implemented after the main source code)

## Goals

- Understand and apply fundamental ray tracing concepts
- Experiment with scene composition, camera placement, and material design
- Extend the basic ray tracer with creative and original features

## Usage

1. Build the project using CMake and your C++ compiler.
2. Run the executable to generate a PPM image file.
3. View the output image with a compatible viewer.

## References

- [Ray Tracing In One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
- [Ray Tracing: The Next Week](https://raytracing.github.io/books/RayTracingTheNextWeek.html)
- [Ray Tracing: The Rest of Your Life](https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html)

---


## Running

### Dependencies

- CMake (version 4.1.0)
- C++17 compatible compiler (e.g., MSVC, GCC, Clang)

### Build and Run Steps

1. Open a terminal in the project root directory.
2. Configure the project:
   ```powershell
   cmake -B build
   ```
3. Build the project:
   ```powershell
   cmake --build build
   ```
4. Run the executable and generate the image:
   ```powershell
   cd build/Debug
   .\main.exe > image.ppm
   cd ../..
   ```
5. Open `image.ppm` with a compatible PPM viewer.
