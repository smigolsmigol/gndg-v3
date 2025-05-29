#pragma once
#include <string>
#include <glad/glad.h>

GLuint LoadShaderFromFile(const std::string& vertexPath, const std::string& fragmentPath);
