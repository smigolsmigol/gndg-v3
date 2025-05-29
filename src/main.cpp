// src/main.cpp
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "tiny_gltf.h"
#include "shader_utils.h"

GLuint VAO = 0, VBO = 0, NBO = 0;
int vertexCount = 0;

bool loadGLTFModel(const std::string& path) {
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err, warn;

    bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, path);
    if (!warn.empty()) std::cout << "Warn: " << warn << std::endl;
    if (!err.empty()) std::cerr << "Err: " << err << std::endl;
    if (!ret) {
        std::cerr << "Failed to load GLTF\n";
        return false;
    }

    const auto& mesh = model.meshes[0];
    const auto& primitive = mesh.primitives[0];

    const auto& posAcc = model.accessors[primitive.attributes.at("POSITION")];
    const auto& posView = model.bufferViews[posAcc.bufferView];
    const auto& posBuffer = model.buffers[posView.buffer];
    const float* positions = reinterpret_cast<const float*>(&posBuffer.data[posAcc.byteOffset + posView.byteOffset]);

    const auto& normAcc = model.accessors[primitive.attributes.at("NORMAL")];
    const auto& normView = model.bufferViews[normAcc.bufferView];
    const auto& normBuffer = model.buffers[normView.buffer];
    const float* normals = reinterpret_cast<const float*>(&normBuffer.data[normAcc.byteOffset + normView.byteOffset]);

    vertexCount = posAcc.count;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &NBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float), positions, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, NBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float), normals, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);

    return true;
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(1280, 720, "GndG3.0 GLTF", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glEnable(GL_DEPTH_TEST);
    GLuint shaderProgram = createShaderProgram("shaders/basic.vert", "shaders/basic.frag");
    loadGLTFModel("assets/my_scene.glb");

    glm::vec3 cameraPos(0.0f, 0.0f, 5.0f);
    glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1280.0f/720.0f, 0.1f, 100.0f);

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, glm::value_ptr(cameraPos));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
