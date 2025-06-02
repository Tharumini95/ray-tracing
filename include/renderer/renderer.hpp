#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

struct Camera;

class Renderer {
private:
	GLuint m_shaderProgram = 0;
	GLuint m_VAO = 0;
	GLuint m_VBO = 0;

	int m_width;
	int m_height;

	GLint m_uniformLocCameraPos;
	GLint m_uniformLocCameraForward;
	GLint m_uniformLocCameraRight;
	GLint m_uniformLocCameraUp;

	void setupShaders();
	void setupQuad();

	void cleanup();

public:
	Renderer(int width, int height);
	~Renderer();

	void render(const Camera& camera);
};