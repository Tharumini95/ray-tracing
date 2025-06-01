#include <fstream>
#include <iostream>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera\camera.hpp"

// === GLOBALS ===
Camera cam;
double lastX = 400.0f, lastY = 300.0f;
bool mouseHeld = false;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

const float MOUSE_SENSITIVITY = 0.3f;
const float CAMERA_SPEED = 5.0f;

// === UTILITY FUNCTIONS ===
std::string readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file: " << path << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLuint createShaderProgram(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexCode = readFile(vertexPath);
    std::string fragmentCode = readFile(fragmentPath);

    if (vertexCode.empty() || fragmentCode.empty()) {
        std::cerr << "Error: Failed to read shader files." << std::endl;
        return 0;
    }

    const char* vertexCodePtr = vertexCode.c_str();
    const char* fragmentCodePtr = fragmentCode.c_str();

    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexCodePtr, nullptr);
    glCompileShader(vertex);

    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentCodePtr, nullptr);
    glCompileShader(fragment);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return program;
}

// === INPUT HANDLING ===
void processInput(GLFWwindow* window) {
    float velocity = CAMERA_SPEED * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        cam.position += cam.forward * velocity;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        cam.position -= cam.forward * velocity;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        cam.position -= cam.right * velocity;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        cam.position += cam.right * velocity;
}

void cursorPosCallback(GLFWwindow* window, double x, double y) {
    if (!mouseHeld) return;

    double xOffset = x - lastX;
    double yOffset = y - lastY;
    lastX = x;
    lastY = y;

    cam.yaw += xOffset * MOUSE_SENSITIVITY;
    cam.pitch += yOffset * MOUSE_SENSITIVITY;

    if (cam.pitch > 89.0f) cam.pitch = 89.0f;
    if (cam.pitch < -89.0f) cam.pitch = -89.0f;

    cam.updateOrientation();
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT)
        if (action == GLFW_PRESS)
        {
            mouseHeld = true;
            glfwGetCursorPos(window, &lastX, &lastY);
        }
        else if (action == GLFW_RELEASE)
            mouseHeld = false;
}

// === MAIN PROGRAM ===
int main() {
    // Init GLFW and OpenGL context
    if (!glfwInit()) {
        std::cerr << "Failed to initalise GLFW" << std::endl;
        return -1;
    }

    // Configure OpenGL context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "ray-tracing", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    // Load OpenGL functions
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "Failed to initialise GLAD" << std::endl;
        return -1;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    glViewport(0, 0, 800, 600);

    // Create shader program
    GLuint shader = createShaderProgram("shaders/vertex.glsl", "shaders/fragment.glsl");
    if (shader == 0) {
        std::cerr << "Failed to create shader program" << std::endl;
        return -1;
    }

    // Fullscreen quad
    float vertices[] = {
        // x, y, z, uv
        -1.0f, -1.0f, 1.0f, 0.0f,  // Bottom-left
        1.0f, -1.0f, 0.0f, 0.0f,  // Bottom-right
        -1.0f, 1.0f, 1.0f, 1.0f,  // Top-left
        1.0f, 1.0f, 0.0f, 1.0f  // Top-right
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute (location 0)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // UV attribute (location 1)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Calculate delta time
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Handle input
        processInput(window);

        // Render
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shader);

        // Upload camera uniforms
        glUniform3fv(glGetUniformLocation(shader, "uCameraPosition"), 1, glm::value_ptr(cam.position));
        glUniform3fv(glGetUniformLocation(shader, "uCameraForward"), 1, glm::value_ptr(cam.forward));
        glUniform3fv(glGetUniformLocation(shader, "uCameraRight"), 1, glm::value_ptr(cam.right));
        glUniform3fv(glGetUniformLocation(shader, "uCameraUp"), 1, glm::value_ptr(cam.up));

        // Draw fullscreen quad
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shader);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}