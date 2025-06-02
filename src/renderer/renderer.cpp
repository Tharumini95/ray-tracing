#include <fstream>
#include <iostream>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>

#include "camera/camera.hpp"
#include "renderer/renderer.hpp"
#include "utils/io.hpp"
#include "utils/shader.hpp"

Renderer::Renderer(int width, int height)
	: m_width(width), m_height(height) {
	setupShaders();
	setupQuad();
}

Renderer::~Renderer() {
	cleanup();
}

void Renderer::setupShaders() {
    m_shaderProgram = createShaderProgram("shaders/vertex.glsl", "shaders/fragment.glsl");
    if (m_shaderProgram == 0) {
        std::cerr << "Failed to create shader program" << std::endl;
    }

    // Cache uniform locations
    m_uniformLocCameraPos = glad_glGetUniformLocation(m_shaderProgram, "uCameraPosition");
    m_uniformLocCameraForward = glGetUniformLocation(m_shaderProgram, "uCameraForward");
    m_uniformLocCameraRight = glGetUniformLocation(m_shaderProgram, "uCameraRight");
    m_uniformLocCameraUp = glGetUniformLocation(m_shaderProgram, "uCameraUp");
}

void Renderer::setupQuad() {
    // Fullscreen quad vertices
    float vertices[] = {
        // x, y, u, v
        -1.0f, -1.0f, 1.0f, 0.0f,  // Bottom-left
        1.0f, -1.0f, 0.0f, 0.0f,  // Bottom-right
        -1.0f, 1.0f, 1.0f, 1.0f,  // Top-left
        1.0f, 1.0f, 0.0f, 1.0f  // Top-right
    };

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute (location 0)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // UV attribute (location 1)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Renderer::render(const Camera& camera) {
    if (m_shaderProgram == 0) return;

    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(m_shaderProgram);

    // Upload camera uniforms
    glUniform3fv(m_uniformLocCameraPos, 1, glm::value_ptr(camera.position));
    glUniform3fv(m_uniformLocCameraForward, 1, glm::value_ptr(camera.forward));
    glUniform3fv(m_uniformLocCameraRight, 1, glm::value_ptr(camera.right));
    glUniform3fv(m_uniformLocCameraUp, 1, glm::value_ptr(camera.up));

    // Draw fullscreen quad
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Renderer::cleanup() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteProgram(m_shaderProgram);
}