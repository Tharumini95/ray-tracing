# Ray Tracing 📽️

Welcome to the **Ray Tracing** repository! This project explores the fascinating world of ray tracing, a rendering technique that simulates the way light interacts with objects to produce realistic images. 

## Table of Contents

- [Introduction](#introduction)
- [Mathematical Foundations](#mathematical-foundations)
  - [Ray Equation](#ray-equation)
  - [Sphere Intersection](#sphere-intersection)
- [Implementation Details](#implementation-details)
- [Getting Started](#getting-started)
- [Examples](#examples)
- [Contributing](#contributing)
- [License](#license)
- [Releases](#releases)

## Introduction

Ray tracing is a powerful technique used in computer graphics to create high-quality images. By tracing the path of rays as they travel through a scene, this method can produce stunning visual effects such as reflections, refractions, and shadows. This repository provides a basic implementation of ray tracing in C++, utilizing modern libraries like GLFW and GLM for graphics and math operations.

## Mathematical Foundations

Understanding the mathematics behind ray tracing is crucial for implementing it effectively. Here are some key equations that form the basis of ray tracing.

### Ray Equation

The equation for a ray can be expressed as:

- **Ray(dst) = Position + Direction * dst**

Where:
- **Position** is the origin vector of the ray.
- **Direction** is a unit vector (magnitude = 1) indicating where the ray points.
- **dst** is a scalar distance along the ray.

### Sphere Intersection

To determine when a ray intersects a sphere, we use the following equations:

#### Sphere Centered at Origin

The equation for a sphere centered at the origin is:

- **x² + y² + z² = r²**

When a ray intersects the boundary of the sphere, this equation holds true:

- **||Position + Direction * dst||² = r²**

#### Sphere Centered at C = (Cx, Cy, Cz)

For a sphere centered at a point C, the equation is:

- **(x - Cx)² + (y - Cy)² + (z - Cz)² = r²**

Again, when a ray intersects the boundary of the sphere, this equation is valid:

- **||(Position + Direction * dst) - C||² = r²**

### Solving for Distance

To find the distance `dst`, we rearrange the equation to solve a quadratic equation:

- **dst² + 2 * (OC · Direction) * dst + (OC · OC - r²) = 0**

Where:
- **OC = Position - C**

The discriminant of this quadratic is:

- **Discriminant = b² - 4ac**
- **= (OC · Direction)² - (OC · OC - r²)**

The conditions for intersection are:
- **b² - 4ac > 0**: Two solutions exist; the ray enters and exits the sphere.

## Implementation Details

This project is structured to facilitate understanding and extendability. Here’s a brief overview of the implementation:

- **C++17**: The code uses C++17 features for modern programming practices.
- **GLFW**: This library manages the window and input for OpenGL.
- **GLM**: GLM provides a set of mathematics functions tailored for graphics.
- **GLSL**: The shading language for writing shaders.

### Directory Structure

```
ray-tracing/
├── src/
│   ├── main.cpp
│   ├── ray.h
│   ├── sphere.h
│   └── renderer.h
├── shaders/
│   ├── vertex.glsl
│   └── fragment.glsl
└── README.md
```

## Getting Started

To get started with the ray tracing project, follow these steps:

1. **Clone the Repository**

   Use the following command to clone the repository:

   ```bash
   git clone https://github.com/Tharumini95/ray-tracing.git
   ```

2. **Install Dependencies**

   Make sure you have the necessary libraries installed. You can find installation instructions for GLFW and GLM in their respective documentation.

3. **Build the Project**

   Navigate to the project directory and build the project using CMake:

   ```bash
   cd ray-tracing
   mkdir build
   cd build
   cmake ..
   make
   ```

4. **Run the Application**

   After building, you can run the application:

   ```bash
   ./ray-tracing
   ```

## Examples

The repository includes several examples demonstrating the capabilities of the ray tracer. These examples cover different scenarios, including:

- Basic sphere rendering
- Reflections and refractions
- Shadows and lighting effects

You can find the example code in the `src` directory. Each example is well-commented to help you understand the logic behind the implementation.

## Contributing

Contributions are welcome! If you would like to contribute to the project, please follow these steps:

1. Fork the repository.
2. Create a new branch for your feature or bug fix.
3. Make your changes and commit them.
4. Push to your branch.
5. Create a pull request.

Please ensure your code follows the project's style guidelines and includes tests where applicable.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Releases

For the latest releases, visit the [Releases section](https://github.com/Tharumini95/ray-tracing/releases). Here, you can download the latest version and find any updates or bug fixes. 

To ensure you have the most up-to-date features and improvements, check this section regularly. 

---

Feel free to explore the code, run the examples, and modify the project to suit your needs. Happy coding!