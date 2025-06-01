#version 330 core

in vec2 vUV;
out vec4 FragColour;

uniform vec3 uCameraPosition;
uniform vec3 uCameraForward;
uniform vec3 uCameraRight;
uniform vec3 uCameraUp;

void main() {
	vec2 uv = vUV * 2.0 - 1.0;  // Normalise uv to [-1,1]
	vec3 rayDir = normalize(uCameraForward + uv.x * uCameraRight + uv.y * uCameraUp);
	vec3 colour = max(rayDir, vec3(0.0));  // Clip negatives
	FragColour = vec4(colour, 1.0);
}