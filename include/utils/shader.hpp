#pragma once

#include <string>

#include <GLFW/glfw3.h>

GLuint createShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);