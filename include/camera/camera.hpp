#pragma once

#include <glm/glm.hpp>

struct Camera {
	glm::vec3 position;
	float yaw;
	float pitch;

	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;

	Camera();

	void updateOrientation();
};