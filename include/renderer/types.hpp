#pragma once

#include <glm/glm.hpp>

struct alignas(16) Material {
	glm::vec3 colour;
	float _pad0;
};

struct alignas(16) Sphere {
	glm::vec3 position;
	float radius;
	Material material;
};