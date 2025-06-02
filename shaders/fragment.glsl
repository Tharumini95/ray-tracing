#version 430 core

in vec2 vUV;
out vec4 FragColour;

uniform vec3 uCameraPosition;
uniform vec3 uCameraForward;
uniform vec3 uCameraRight;
uniform vec3 uCameraUp;

struct Ray {
	vec3 origin;
	vec3 dir;
};

struct Material {
	vec3 colour;
	float _pad0;
};

struct Sphere {
	vec3 position;
	float radius;
	Material material;
};

layout(std430, binding = 0) buffer Spheres {
	Sphere spheres[];
};

uniform int uNumSpheres;

bool RaySphereIntersect(Ray ray, vec3 centre, float radius, out float dst) {
	vec3 oc = ray.origin - centre;
	float a = dot(ray.dir, ray.dir);
	float b = 2.0 * dot(oc, ray.dir);
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - 4.0 * a * c;

	if (discriminant < 0.0)
		return false;

	dst = (-b - sqrt(discriminant)) / (2.0 * a);
	return dst >= 0.0;
}

void main() {
	vec2 uv = vUV * 2.0 - 1.0;  // Normalise uv to [-1,1]

	// Construct ray
	Ray ray;
	ray.origin = uCameraPosition;
	ray.dir = normalize(uCameraForward - uv.x * uCameraRight + uv.y * uCameraUp);

	float shortestDst = 1e20;
	vec3 hitColour = vec3(0.0);
	bool didHit = false;

	// Iterate spheres and check for ray intersections
	for (int i = 0; i < uNumSpheres; ++i) {
		float dst;
		if (RaySphereIntersect(ray, spheres[i].position, spheres[i].radius, dst)) {
			// Only care about the visible intersection
			if (dst < shortestDst) {
				shortestDst = dst;
				hitColour = spheres[i].material.colour;
				didHit = true;
			}
		}
	}

	if (didHit) {
		FragColour = vec4(hitColour, 1.0);
	} else {
		FragColour = vec4(0.0, 0.0, 0.0, 1.0);
	}
}