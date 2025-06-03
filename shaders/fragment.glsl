#version 430 core

in vec2 vUV;
out vec4 FragColour;

uniform vec2 uResolution;

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
	vec3 emission;
	float _pad1;
};

struct Sphere {
	vec3 position;
	float radius;
	Material material;
};

struct HitInfo {
	bool hit;
	float dst;
	vec3 hitPoint;
	vec3 normal;
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

HitInfo CalculateRayCollision(Ray ray) {
	HitInfo closestHit;
	closestHit.hit = false;
	closestHit.dst = 1e20;

	// Find closest intersection
	for (int i = 0; i < uNumSpheres; ++i) {
		float dst;
		if (RaySphereIntersect(ray, spheres[i].position, spheres[i].radius, dst)) {
			if (dst > 0.0 && dst < closestHit.dst) {
				closestHit.hit = true;
				closestHit.dst = dst;
				closestHit.hitPoint = ray.origin + ray.dir * dst;
				closestHit.normal = normalize(closestHit.hitPoint - spheres[i].position);
				closestHit.material = spheres[i].material;
			}
		}
	}

	return closestHit;
}

vec3 CalculateLighting(HitInfo hit) {
	vec3 lightColour = vec3(0.0);

	// Find all light sources
	for (int i = 0; i < uNumSpheres; ++i) {
		if (spheres[i].material.emission != vec3(0.0)) {
			vec3 lightPos = spheres[i].position;
			vec3 lightDir = normalize(lightPos - hit.hitPoint);
			float lightDst = length(lightPos - hit.hitPoint);

			float dotProduct = max(dot(hit.normal, lightDir), 0.0);

			lightColour += spheres[i].material.emission * hit.material.colour * dotProduct;
		}
	}

	return lightColour;
}

vec3 Trace(Ray ray) {
	HitInfo hit = CalculateRayCollision(ray);

	if (!hit.hit) {
		return vec3(0.0);
	}

	if (hit.material.emission != vec3(0.0)) {
		return hit.material.emission;
	}

	vec3 lighting = CalculateLighting(hit);

	return lighting;
}

void main() {
	vec2 uv = vUV * 2.0 - 1.0;  // Normalise uv to [-1,1]
	uv.x *= uResolution.x / uResolution.y;

	Ray ray;
	ray.origin = uCameraPosition;
	ray.dir = normalize(uCameraForward - uv.x * uCameraRight + uv.y * uCameraUp);

	vec3 colour = Trace(ray);

	FragColour = vec4(colour, 1.0);
}