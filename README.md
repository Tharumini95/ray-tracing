# ray-tracing

## Ray Tracing Maths

-   Equation for Ray

    -   Ray(dst) = Position + Direction \* dst
    -   Position is the origin vector of the ray
    -   Direction is a unit vector (magnitude = 1) where the ray points towards
    -   dst is a scalar distance along the ray

-   Equation for Sphere, centred at origin

    -   x² + y² + z² = r²
    -   When a ray intersects the boundary of the sphere, this equation is true
        -   ||Position + Direction \* dst||² = r²

-   Equation for Sphere, centred at C = (Cx, Cy, Cz)

    -   (x - Cx)² + (y - Cy)² + (z - Cz)² = r²
    -   When a ray intersects the boundary of the sphere, this equation is true
        -   ||(Position + Direction \* dst) - C||² = r²

-   Re-arrange to solve a quadratic for dst
    -   dst² + 2 \* (OC · Direction) \* dst + (OC · OC - r²) = 0
        -   Where OC = Position - C
    -   Discriminant = b² - 4ac
        -   = (OC · Direction)² - (OC · OC - r²)
        -   b² - 4ac > 0, two solutions, ray enters and exits the sphere
        -   b² - 4ac = 0, one solution, ray just touches the sphere (tangent)
        -   b² - 4ac < 0, no real solutions, no intersection

-   Lambertian (Diffuse) Lighting
    -   Diffuse = LightColour \* SurfaceColour \* max(Normal · LightDir, 0)